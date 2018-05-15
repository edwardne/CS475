#include <cstdio>
#include <cstdlib>

#define TABLEPRINT 0

//STATE 
int    NowYear;        // 2017 - 2022
int    NowMonth;        // 0 - 11

float    NowPrecip;        // inches of rain per month
float    NowTemp;        // temperature this month
float    NowHeight;        // grain height in inches
int    NowNumDeer;        // number of deer in the current population

// Time steps
const float GRAIN_GROWS_PER_MONTH =        8.0;
const float ONE_DEER_EATS_PER_MONTH =        0.5;

const float AVG_PRECIP_PER_MONTH =        6.0;    // average
const float AMP_PRECIP_PER_MONTH =        6.0;    // plus or minus
const float RANDOM_PRECIP =            2.0;    // plus or minus noise

const float AVG_TEMP =                50.0;    // average
const float AMP_TEMP =                20.0;    // plus or minus
const float RANDOM_TEMP =            10.0;    // plus or minus noise

const float MIDTEMP =                40.0;
const float MIDPRECIP =                10.0;

// Function Prototypes
void GrainDeer();
void Grain();
void Watcher();
void MyAgent();
float SQR( float x );
float Ranf( unsigned int *seedp,  float low, float high );
int Ranf( unsigned int *seedp, int ilow, int ihigh );
void Print();

int main () {
    // starting values
    // starting date and time:
    NowMonth =    0;
    NowYear  = 2017;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;


    omp_set_num_threads( 3 );    // same as # of sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            GrainDeer( );
        }

        #pragma omp section
        {
            Grain( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        /* #pragma omp section */
        /*         { */
        /*             MyAgent( );    // your own */
        /*         } */
        /*     }       // implied barrier -- all functions must return in order */

        return 0;
}

GrainDeer(){
    while( NowYear < 2023 ) {
        int tempNowNumDeer = NowNumDeer;

        if(NowHeight > NowNumDeer){
            tempNowNumDeer--
        } else if (NowHeight < NowNumDeer){
            tempNowNumDeer++;
        }
        if( tempNowNumDeer < 0. ) tempNowNumDeer = 0.;

        // DoneComputing barrier:
        #pragma omp barrier

        NowNumDeer = tempNowNumDeer;

        // DoneAssigning barrier:
        #pragma omp barrier

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

Grain(){
    while( NowYear < 2023 ) {
        //Calculate Grain height
        float tempNowHeight = NowHeight;
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );


        tempNowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
        tempNowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        if( tempNowHeight < 0. ) tempNowHeight = 0.;

        // DoneComputing barrier:
        #pragma omp barrier

        NowHeight = tempNowHeight;

        // DoneAssigning barrier:
        #pragma omp barrier

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

Watcher(){
    while( NowYear < 2023 ) {
        // DoneComputing barrier:
        #pragma omp barrier
        // DoneAssigning barrier:
        #pragma omp barrier

        //Print State
        printState();

        //Temperature and Precipitation calculations
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        unsigned int seed = 0;
        NowTemp = temp + Ranf( &seed;, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed;,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. ) NowPrecip = 0.;

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

MyAgent(){
    while( NowYear < 2023 ) {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        // DoneComputing barrier:
        #pragma omp barrier

        // DoneAssigning barrier:
        #pragma omp barrier

        // DonePrinting barrier:
        #pragma omp barrier
    }
}

// Utility Functions
float SQR( float x ) {
        return x*x;
}

float Ranf( unsigned int *seedp,  float low, float high ) {
    float r = (float) rand_r( seedp );              // 0 - RAND_MAX

    return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int Ranf( unsigned int *seedp, int ilow, int ihigh ) {
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;
    return (int)(  Ranf(seedp, low,high) );
}
void Print() {
    if (TABLEPRINT == 1){
        printf("%8.2lf\t%8.2lf\t%8d\t%8.2lf\t\n", NowTemp, NowPrecip, NowNumDeer, NowGrain);
    } else {
        printf("YEAR %d MONTH %d\n", NowYear, NowMonth);
        printf("TEMP %f PRECIP %f\n", NowTemp, NowPrecip);
        printf("DEER %d GRAIN %f\n", NowNumDeer, NowGrain);
    }
}
