source_filename = "./examples\floats\floats.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
  %pi = alloca float
  store float 0x400922d0e0000000, float* %pi
  %r = alloca float
  store float 2.0, float* %r
  %1 = load float, float* %pi
  %2 = load float, float* %r
  %3 = fmul float %1, %2
  %4 = load float, float* %r
  %5 = fmul float %3, %4
  %area = alloca float
  store float %5, float* %area
  %6 = load float, float* %area
  %7 = fpext float %6 to double
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %7)
  %9 = load float, float* %area
  %10 = fcmp oeq float %9, 12.0
  %11 = zext i1 %10 to i8
  %eq = alloca i8
  store i8 %11, i8* %eq
  %12 = load float, float* %area
  %13 = fcmp ogt float %12, 12.0
  %14 = zext i1 %13 to i8
  %gt = alloca i8
  store i8 %14, i8* %gt
  %x = alloca i32
  store i32 15, i32* %x
  %15 = load float, float* %pi
  %16 = load i32, i32* %x
  %17 = sitofp i32 %16 to float
  %18 = fmul float %15, %17
  %19 = load i32, i32* %x
  %20 = sitofp i32 %19 to float
  %21 = fmul float %18, %20
  %area1 = alloca float
  store float %21, float* %area1
  %22 = load float, float* %area1
  %23 = fpext float %22 to double
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %23)
  %y = alloca double
  store double 12.0, double* %y
  %25 = load float, float* %pi
  %26 = load double, double* %y
  %27 = fpext float %25 to double
  %28 = fmul double %27, %26
  %29 = load double, double* %y
  %30 = fmul double %28, %29
  %area2 = alloca double
  store double %30, double* %area2
  %31 = load double, double* %area2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %31)
  %a = alloca double
  store double 12.34, double* %a
  %33 = load double, double* %y
  %34 = fptrunc double %33 to float
  %b = alloca float
  store float %34, float* %b
  %35 = load float, float* %b
  %36 = fsub float 0.0, %35
  %37 = fpext float %36 to double
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %37)
  ret i32 0
}
