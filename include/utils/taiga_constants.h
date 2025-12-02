#ifndef CONSTANTS_H
#define CONSTANTS_H
    #define PI 3.141592653589792346
    #define ELEMENTARY_CHARGE 1.60217656535e-19
    #define AMU 1.66053892173e-27
    #define MASS_OF_ELECTRON 0.000549
    #define INFINITY_INTEGER RAND_MAX

    #define X_SIZE 8
    #define BEAMLET_INTENSITY_ID 6
    #define TIME_OF_FLIGHT_ID 7

    #define UNDEFINED_FLOAT -1e99
    #define CALCULATION_NOT_FINISHED -1
    #define CALCULATION_FINISHED 0
    #define OUT_OF_RANGE -1
    #define STRING_LENGTH 100

    #define UNDEFINED_RUNNUMBER "/"

    #define SOLVER_RK45 0
    #define SOLVER_RUNGE_KUTTA_NYSTROM 1
    #define SOLVER_VERLET 2
    #define SOLVER_YOSHIDA 4

    #define CUBIC_SPLINE 3
    #define CUBIC_BSPLINE 13

    #define LINEAR_INTERPOLATION 0
    #define BEZIER_INTERPOLATION 1

    #define SPLINE_INDEX_ERROR -1

    #define SERVICE_VAR_LENGTH 10

    #define HELP_MODE 1
    #define HELP_DEVICES 2
    #define HELP_VERSION 3

    #define ALL_IO 0
    #define JUST_WRITE 1
    #define MINIMAL_IO 2
    #define NO_TRACE 3

    #define READ_COORDINATES 0
    #define READ_RENATE_OD 1

    #define PERT_OFF 0
    #define PERT_ON 1


#pragma once
#include "core/policies.cuh"

using KernelFn = void(*)(dim3, dim3, TaigaGlobals*, TaigaCommons*, double*);

#define ENTRY(SOLVER, FIELD, ION, EF, DET, PERT) \
    &launch_kernel<SOLVER, FIELD, ION, EF, DET, PERT>

#define SOLVER_CASES(FIELD, ION, EF, DET, PERT) \
    { ENTRY(SolverRK4, FIELD, ION, EF, DET, PERT), \
      ENTRY(SolverRKN, FIELD, ION, EF, DET, PERT), \
      ENTRY(SolverVerlet, FIELD, ION, EF, DET, PERT), \
      ENTRY(SolverYoshida, FIELD, ION, EF, DET, PERT) }

#define FIELD_CASES(ION, EF, DET, PERT) \
    { SOLVER_CASES(CubicSplineInterp, ION, EF, DET, PERT), \
      SOLVER_CASES(CubicBSplineInterp, ION, EF, DET, PERT) }

#define ION_CASES(EF, DET, PERT) \
    { FIELD_CASES(IonisationOff, EF, DET, PERT), \
      FIELD_CASES(IonisationOn,  EF, DET, PERT) }

#define EF_CASES(DET, PERT) \
    { ION_CASES(LorentzWithoutElectric, DET, PERT), \
      ION_CASES(LorentzWithElectric,  DET, PERT) }

#define DET_CASES(PERT) \
    { EF_CASES(LinearInterpolation,  PERT), \
      EF_CASES(BezierInterpolation,  PERT), \
      EF_CASES(HermiteInterpolation, PERT) }

#define PERT_CASES \
    { DET_CASES(PerturbationOff), \
      DET_CASES(PerturbationOn) }

// Dimensions: [solver][field][ion][ef][pert][detect]
static KernelFn taiga_dispatch_table[4][2][2][2][2][3] = PERT_CASES;

#endif //CONSTANTS_H