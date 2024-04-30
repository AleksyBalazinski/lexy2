source_filename = "./examples\arithmetic\arithmetic.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define dso_local i32 @main() #0 {
  %1 = add i32 3, 5
  %2 = sub i32 0, 2
  %3 = mul i32 %1, %2
  %4 = sitofp i32 %3 to double
  %5 = fdiv double %4, 2.15
  %6 = sub i32 0, 2
  %7 = sitofp i32 %6 to double
  %8 = fmul double %7, 3.143
  %9 = fmul double 345.0e-3, %8
  %10 = fadd double %5, %9
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %10)
  %12 = fmul double 3.14, 8.0
  %13 = fptosi double %12 to i32
  %14 = sitofp i32 %13 to double
  %15 = fmul double %14, 2.0
  %16 = fdiv double %15, 3.0
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %16)
  %18 = fadd double 3.0, 3.0
  %19 = fsub double 0.0, 1.12
  %20 = fmul double 45.0, %19
  %21 = fadd double %18, %20
  %22 = fmul double 12.5, %21
  %23 = fdiv double 128.0, 3.0
  %24 = sub i32 0, 45
  %25 = mul i32 %24, 23
  %26 = sitofp i32 %25 to double
  %27 = fadd double %26, 12.123
  %28 = fmul double %27, 1.0e-3
  %29 = fadd double 125.0, %28
  %30 = fmul double %23, %29
  %31 = fadd double %22, %30
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %31)
  ret i32 0
}
