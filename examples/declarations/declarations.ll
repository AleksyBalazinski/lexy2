source_filename = "./examples\declarations\declarations.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define dso_local i32 @main() #0 {
  %acceleration = alloca double
  store double 9.8, double* %acceleration
  %time = alloca double
  store double 3.2e-2, double* %time
  %initial_velocity = alloca i32
  store i32 2, i32* %initial_velocity
  %1 = load i32, i32* %initial_velocity
  %2 = load double, double* %time
  %3 = sitofp i32 %1 to double
  %4 = fmul double %3, %2
  %5 = load double, double* %acceleration
  %6 = fmul double 0.5, %5
  %7 = load double, double* %time
  %8 = fmul double %6, %7
  %9 = load double, double* %time
  %10 = fmul double %8, %9
  %11 = fadd double %4, %10
  %distance = alloca double
  store double %11, double* %distance
  %12 = fsub double 0.0, 5.2
  %initial_dist = alloca double
  store double %12, double* %initial_dist
  %13 = load double, double* %distance
  %14 = load double, double* %initial_dist
  %15 = fadd double %13, %14
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %15)
  ret i32 0
}
