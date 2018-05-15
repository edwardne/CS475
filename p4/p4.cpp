
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

//
void GrainDeer();
void Grain();
void Watcher();
void MyAgent();

int main () {

omp_set_num_threads( 4 );    // same as # of sections
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

    #pragma omp section
    {
        MyAgent( );    // your own
    }
}       // implied barrier -- all functions must return in order

}

//Temperature and Precipitation calculations
float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

float temp = AVG_TEMP - AMP_TEMP * cos( ang );
unsigned int seed = 0;
NowTemp = temp + Ranf( &seed;, -RANDOM_TEMP, RANDOM_TEMP );

float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
NowPrecip = precip + Ranf( &seed;,  -RANDOM_PRECIP, RANDOM_PRECIP );
if( NowPrecip < 0. ) NowPrecip = 0.;

// starting values
// starting date and time:
NowMonth =    0;
NowYear  = 2017;

// starting state (feel free to change this if you want):
NowNumDeer = 1;
NowHeight =  1.;
