source_filename = "./examples\declarations\declarations.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %acceleration = alloca double
  store double 9.8, double* %acceleration
  %time = alloca double
  store double 3.2e-2, double* %time
  %initial_velocity = alloca i32
  store i32 2, i32* %initial_velocity
  %0 = load i32, i32* %initial_velocity
  %1 = load double, double* %time
  %2 = sitofp i32 %0 to double
  %3 = fmul double %2, %1
  %4 = load double, double* %acceleration
  %5 = fmul double 0.5, %4
  %6 = load double, double* %time
  %7 = fmul double %5, %6
  %8 = load double, double* %time
  %9 = fmul double %7, %8
  %10 = fadd double %3, %9
  %distance = alloca double
  store double %10, double* %distance
  %11 = fsub double 0.0, 5.2
  %initial_dist = alloca double
  store double %11, double* %initial_dist
  %12 = load double, double* %distance
  %13 = load double, double* %initial_dist
  %14 = fadd double %12, %13
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %14)
  ret i32 0
}
