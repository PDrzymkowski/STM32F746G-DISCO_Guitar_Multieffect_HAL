/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : phase_vocoder.c
  * @brief         : Algorytm wokodera fazowego do przesuniecia
	*											sygnalu w wysokosci
  ******************************************************************************
  */
/* USER CODE END Header */

#include "phase_vocoder.h"


arm_rfft_fast_instance_f32 fftInstance;

// Bufor cykliczny do przechowywania próbek
float32_t x_in_temp[FFT_SIZE];
float32_t x_out_temp[OUT_TEMP_BUF_SIZE];
float32_t x_out_resampled[X_OUT_RESAMPLED_SIZE];
uint16_t inTempPointer;
uint16_t outTempPointer;

// Zmienne algorytmu Pitch Shifter

float32_t Rs = Ra;
float32_t Ra_inverse = 1.0/Ra;
float32_t alpha = 2.0;
//float32_t ampScale = 1.0/3.0;
uint16_t numUsefulBins = FFT_SIZE/2;

float32_t hann1024[] = hann_window_1024_samples;
float32_t hann512[512];
float32_t hann_sqrt[] = hann_sqrt_1024_samples;

//float32_t hann512[512];
float32_t hannScaled[1024];
float32_t omegaNominal[FFT_SIZE/2];
float32_t deltaPhiExpected[FFT_SIZE/2];
float32_t x_in[FFT_SIZE];

// Analiza
float32_t X_fft_in[FFT_SIZE];
float32_t A_in[FFT_SIZE/2];
float32_t phi_in[FFT_SIZE/2];

// Modyfikacja
float32_t phiInPrevious[FFT_SIZE/2];
float32_t phiOutPrevious[FFT_SIZE/2];
float32_t deltaPhi[FFT_SIZE/2];
float32_t deltaPhiUnwrap[FFT_SIZE/2];
float32_t omegaIn[FFT_SIZE/2];

// Synteza
float32_t phi_out[FFT_SIZE/2];
float32_t X_fft_out[FFT_SIZE];
float32_t x_out[FFT_SIZE];
arm_linear_interp_instance_f32 linInterpInst = {FFT_SIZE, 0, 1, &x_out[0]};


void PhaseVocoder(int16_t *data, int8_t pitch) 
{
	alpha = powf(2.0f, (float)(pitch)/12);

	FillInputTempBuffer(data);
  PhaseVocoderAnalysis();
  PhaseVocoderModification();
  PhaseVocoderSynthesis();
  FillOutputTempBuffer(data);
}


void PhaseVocoderInit(void) 
{
	uint16_t k, i;
	for (k=0; k<FFT_SIZE/2; k++)
	{
		omegaNominal[k] = (k*2*PI/FFT_SIZE);
		deltaPhiExpected[k] = (k*2*PI/FFT_SIZE) * Ra;
	}
	for (i=0; i<512; i++)
	{
		hann512[i] = hann1024[2*i];
	}
	// Okno syntezy (skalowanie)
	arm_scale_f32(hann1024, alpha, hannScaled, FFT_SIZE);

	// Inicjalizacja FFT
	arm_rfft_fast_init_f32 (&fftInstance, FFT_SIZE);
}


void FillInputTempBuffer(int16_t *data_to_process) 
{
	 // Skopiowanie probek wejsciowych do bufora cyklicznego (konwersja do float)
  arm_q15_to_float (&data_to_process[0], &x_in_temp[inTempPointer], AUDIO_BLOCK_SIZE);

  // Uaktualnienie pozycji w buforze cyklicznym x_in_temp[]
  //inTempPointer = (inTempPointer + FFT_SIZE/4) % FFT_SIZE;
  inTempPointer = (inTempPointer + AUDIO_BLOCK_SIZE) % FFT_SIZE;

  // Wypelnienie x_in (sygnal wejsciowy algorytmu)
  if (inTempPointer == 0)
  {
    arm_copy_f32(x_in_temp, x_in, FFT_SIZE);
  }
  else
  {
	  // Na koncu bufora cyklicznego
	  arm_copy_f32(&x_in_temp[inTempPointer], x_in, FFT_SIZE - inTempPointer);
	  // Na poczatku bufora cyklicznego
	  arm_copy_f32(x_in_temp, &x_in[FFT_SIZE - inTempPointer], inTempPointer);
  }
}

void PhaseVocoderAnalysis(void)
{
	 uint16_t k;
  // Przeskalowanie sygna³u wejœciowego
  //arm_scale_f32(x_in, ampScale, x_in, FFT_SIZE);

  // Wymnozenie sygnalu wejsciowego przez okno analizy (Hanninga)
  arm_mult_f32 (x_in, hann1024, x_in, FFT_SIZE);
  //arm_mult_f32 (x_in, hann_sqrt, x_in, FFT_SIZE);
  //arm_mult_f32 (x_in, hann512, x_in, FFT_SIZE);

  // Obliczenie FFT sygnalu wejsciowego
  arm_rfft_fast_f32 (&fftInstance, x_in, X_fft_in, 0);

  // Amplituda FFT wejsciowego
  //arm_cmplx_mag_f32 (X_fft_in, A_in, FFT_SIZE/2);
  arm_cmplx_mag_f32 (X_fft_in, A_in, numUsefulBins);

  // Faza FFT wejsciowego
  //for (k=0; k < FFT_SIZE/2; k++)
  for (k=0; k < numUsefulBins; k++)
  {
    phi_in[k] = atan2f(X_fft_in[2*k+1], X_fft_in[2*k]);
	//phi_in[k] = Atan2_Approx(X_fft_in[2*k+1], X_fft_in[2*k]);
  }
}


void PhaseVocoderModification() 
{
	 uint16_t k;
  // Roznica faz pomiedzy kolejnymi ramkami
  //arm_sub_f32 (phi_in, phiInPrevious, deltaPhi, FFT_SIZE/2);
  //arm_copy_f32 (phi_in, phiInPrevious, FFT_SIZE/2);
  arm_sub_f32 (phi_in, phiInPrevious, deltaPhi, numUsefulBins);
  arm_copy_f32 (phi_in, phiInPrevious, numUsefulBins);

  //for (k=0; k < FFT_SIZE/2; k++)
  for (k=0; k < numUsefulBins; k++)
  {
    // "Odwijanie" (ang. Unwrapping) fazy
    deltaPhiUnwrap[k] = fmodf( (deltaPhi[k] - deltaPhiExpected[k] + PI), (2*PI));
    if (deltaPhiUnwrap[k] < 0)
      deltaPhiUnwrap[k] += PI;
    else
      deltaPhiUnwrap[k] -= PI;
    omegaIn[k] = omegaNominal[k] + deltaPhiUnwrap[k] / Ra;
    //omegaIn[k] = omegaNominal[k] + deltaPhiUnwrap[k] * Ra_inverse;

    // Obliczenie fazy chwilowej
    //phi_out[k] = fmodf(phiOutPrevious[k] + Rs*omegaIn[k], 2*PI);
    phi_out[k] = fmodf(phi_out[k] + Rs*omegaIn[k], 2*PI);
    //phi_out[k] = phiOutPrevious[k] + Rs*omegaIn[k];

    if (phi_out[k] < 0)
      phi_out[k] += (2*PI);
    phiOutPrevious[k] = phi_out[k];

    // Brak modyfikacji
    //phi_out[k] = phi_in[k];
    // Widmo sygnalu wyjsciowego
    X_fft_out[2*k] = A_in[k] * arm_cos_f32(phi_out[k]);
    X_fft_out[2*k+1] = A_in[k] * arm_sin_f32(phi_out[k]);
  }
  // Brak modyfikacji
  //arm_copy_f32 (X_fft_in, X_fft_out, FFT_SIZE);
}

void PhaseVocoderSynthesis()
{
	  uint16_t i;
  // Obliczenie odwrotnej FFT
  arm_rfft_fast_f32 (&fftInstance, X_fft_out, x_out, 1);

  // Wymno¿enie przez okno Hanninga
  arm_mult_f32 (x_out, hannScaled, x_out, FFT_SIZE);
  //arm_mult_f32 (x_out, hann512, x_out, FFT_SIZE);
  //arm_mult_f32 (x_out, hann_sqrt, x_out, FFT_SIZE);
  //arm_mult_f32 (x_out, hann1024, x_out, FFT_SIZE);

  // Przeprobkowanie (Resampling)
  // Brak modyfikacji - skopiowanie wektora x_out[] do x_out_resampled[]
  //arm_copy_f32 (x_out, x_out_resampled, FFT_SIZE);

	/*
  //for (i = 0; i<FFT_SIZE/ALPHA; i++)
  {
	  // Interpolacja liniowa
	  for (i = 0; i<FFT_SIZE/alpha; i++)
	  {
		  x_out_resampled[i] = arm_linear_interp_f32 (&linInterpInst, ((float32_t) i) * alpha);
	  }
  } */
 
  // Interpolacja krzywymmi sklejanymi
  SplineInterp();
  // Dodanie z nakladaniem (Overlap-Add) wektora o dlugosci x_out_resampled_SIZE
  //if (outTempPointer + FFT_SIZE <= OUT_TEMP_BUF_SIZE)
  if (outTempPointer  <= OUT_TEMP_BUF_SIZE - X_OUT_RESAMPLED_SIZE)
  {
    //arm_add_f32 (x_out, &x_out_temp[outTempPointer], &x_out_temp[outTempPointer], FFT_SIZE);
    arm_add_f32 (x_out_resampled, &x_out_temp[outTempPointer], &x_out_temp[outTempPointer], X_OUT_RESAMPLED_SIZE);
  }
  else
  {
    // Na koncu bufora cyklicznego
    //arm_add_f32 (x_out, &x_out_temp[outTempPointer], &x_out_temp[outTempPointer], OUT_TEMP_BUF_SIZE - outTempPointer);
    arm_add_f32 (x_out_resampled, &x_out_temp[outTempPointer], &x_out_temp[outTempPointer], OUT_TEMP_BUF_SIZE - outTempPointer);
    // Na poczatku bufora cyklicznego
    arm_add_f32 (&x_out_resampled[OUT_TEMP_BUF_SIZE - outTempPointer], x_out_temp, x_out_temp, X_OUT_RESAMPLED_SIZE + outTempPointer - OUT_TEMP_BUF_SIZE);
    //arm_add_f32 (&x_out_resampled[OUT_TEMP_BUF_SIZE - outTempPointer], x_out_temp, x_out_temp, FFT_SIZE + outTempPointer - OUT_TEMP_BUF_SIZE);
  }
}

void FillOutputTempBuffer(int16_t *data_to_process) 
{
	// Wyslanie do bufora nadawczego outputBuffer[] wektora o dlugosci FFT_SIZE/4
  //if (outTempPointer + FFT_SIZE/4 <= OUT_TEMP_BUF_SIZE)
	
  if (outTempPointer + AUDIO_BLOCK_SIZE <= OUT_TEMP_BUF_SIZE)
  {
    //arm_float_to_q15 (&x_out_temp[outTempPointer], &outputBuffer[outPointer], FFT_SIZE/4);
	  arm_float_to_q15 (&x_out_temp[outTempPointer], data_to_process, AUDIO_BLOCK_SIZE);
    // Wyczyszczenie przeslanej czesci bufora cyklicznego x_out_temp[]
    //arm_fill_f32 (0, &x_out_temp[outTempPointer], FFT_SIZE/4);
	  arm_fill_f32 (0, &x_out_temp[outTempPointer], AUDIO_BLOCK_SIZE);
  }
  /*else
  {
    // Na poczatku uzywanej polowy bufora nadawczego
    arm_float_to_q15 (&x_out_temp[outTempPointer], &data[0], OUT_TEMP_BUF_SIZE - outTempPointer);
    arm_fill_f32 (0, &x_out_temp[outTempPointer], OUT_TEMP_BUF_SIZE - outTempPointer);
    // Na koncu polowy bufora nadawczego
    //arm_float_to_q15 (&x_out_temp[outTempPointer + OUT_TEMP_BUF_SIZE - outTempPointer], &outputBuffer[outPointer + OUT_TEMP_BUF_SIZE - outTempPointer], FFT_SIZE/4 + outTempPointer - OUT_TEMP_BUF_SIZE);
    arm_float_to_q15 (&x_out_temp[outTempPointer + OUT_TEMP_BUF_SIZE - outTempPointer], &data[0 + OUT_TEMP_BUF_SIZE - outTempPointer], AUDIO_BLOCK_SIZE + outTempPointer - OUT_TEMP_BUF_SIZE);
    //arm_fill_f32 (0, &x_out_temp[outTempPointer + OUT_TEMP_BUF_SIZE - outTempPointer], FFT_SIZE/4 + outTempPointer - OUT_TEMP_BUF_SIZE);
    arm_fill_f32 (0, &x_out_temp[outTempPointer + OUT_TEMP_BUF_SIZE - outTempPointer], AUDIO_BLOCK_SIZE + outTempPointer - OUT_TEMP_BUF_SIZE);
  }
	*/
  // Aktualizacja wskaznika bufora cyklicznego
  outTempPointer = (outTempPointer + AUDIO_BLOCK_SIZE) % OUT_TEMP_BUF_SIZE;	
}


void SplineInterp() 
{
	uint16_t i, i_src_floor;
	float32_t i_src, x, p0, p1, p2, p3, s;
	for (i = 2; i<(FFT_SIZE-2)/alpha; i++)
	{
		i_src = i*alpha;
		i_src_floor = (uint16_t) floorf(i_src);
		//i_src_floor = (uint16_t) i_src;
		p0 = x_out[i_src_floor - 1];
		p1 = x_out[i_src_floor];
		p2 = x_out[i_src_floor + 1];
		p3 = x_out[i_src_floor + 2];
		x = i_src - floorf(i_src);
		s = x*x;
		//x_out_resampled[i] = (-0.5*p0 + 1.5*p1 - 1.5*p2 + 0.5*p3)*s*x + (p0 - 2.5*p1 + 2*p2 - 0.5*p3)*s + (-0.5*p0 + 0.5*p2)*x + p1;
		//x_out_resampled[i] = (-p0/2.0 + 3.0/2.0*p1 - 3.0/2.0*p2 + p3/2.0)*x*x*x + (p0 - 5.0/2.0*p1 + 2.0*p2 - p3/2.0)*x*x + (-p0/2.0 + p2/2.0)*x + p1;
		x_out_resampled[i] = (-p0/2 + p1*3/2 - p2*3/2 + p3/2)*x*x*x + (p0 - p1*5/2 + 2*p2 - p3/2)*x*x + (-p0/2 + p2/2)*x + p1;
		//x_out_resampled[i] = ((-p0 + p1*3 - p2*3 + p3)*x*x*x + (2*p0 - p1*5 + 4*p2 - p3)*x*x + (-p0 + p2)*x) + 2*p1;
	}
	//arm_fill_f32 (0, &x_out_resampled[i+1], x_out_resampled_SIZE - i - 1);
}


float32_t Atan2_Approx(float32_t y, float32_t x) 
{
	float32_t a, phi, maxXY, minXY, absX, absY;

	// Znalezienie max(|x|,|y|), min(|x|,|y|)
	absX = fabs(x);
	absY = fabs(y);
	if (absX>absY)
	{
		maxXY = absX;
		minXY = absY;
	}else
	{
		maxXY = absY;
		minXY = absX;
	}

	// Przypadki szczególne
	if (x==0)
	{
		if (y>0)
			phi = PI_2;
		else if (y<0)
			phi = -PI_2;
		else
			phi = 0;
	}
	else if (y==0)
	{
		if(x>0)
			phi = 0;
		else
			phi = PI;

	// Aproksymacja atan2(y,x)
	}else
	{
		a = minXY/maxXY;
		//s = a*a;
		//phi = ((-0.0464964749*s + 0.15931422)*s - 0.327622764) * s * a + a;
		phi = (float)(PI_4)*a - a * (a-1) * (0.2447f + 0.0663f*a);
		// atan(z) = PI/2 - atan(1/z)
		if (absY>absX)
			phi = (float)(PI_2) - phi;
		// atan(-z) = -atan(z)
		if (x<0)
			phi = PI - phi;
		if (y<0)
			phi = -phi;
	}
	return phi;
}
