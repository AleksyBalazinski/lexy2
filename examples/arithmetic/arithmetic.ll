source_filename = "./examples\arithmetic\arithmetic.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %0 = add i32 3, 5
  %1 = sub i32 0, 2
  %2 = mul i32 %0, %1
  %3 = sitofp i32 %2 to double
  %4 = fdiv double %3, 2.15
  %5 = sub i32 0, 2
  %6 = sitofp i32 %5 to double
  %7 = fmul double %6, 3.143
  %8 = fmul double 345.0e-3, %7
  %9 = fadd double %4, %8
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %9)
  %11 = sitofp i32 8 to double
  %12 = fmul double 3.14, %11
  %13 = fptosi double %12 to i32
  %14 = sitofp i32 2 to double
  %15 = sitofp i32 %13 to double
  %16 = fmul double %15, %14
  %17 = fdiv double %16, 3.0
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %17)
  %19 = sitofp i32 3 to double
  %20 = fadd double %19, 3.0
  %21 = fsub double 0.0, 1.12
  %22 = sitofp i32 45 to double
  %23 = fmul double %22, %21
  %24 = fadd double %20, %23
  %25 = fmul double 12.5, %24
  %26 = sitofp i32 128 to double
  %27 = fdiv double %26, 3.0
  %28 = sub i32 0, 45
  %29 = mul i32 %28, 23
  %30 = sitofp i32 %29 to double
  %31 = fadd double %30, 12.123
  %32 = fmul double %31, 1.0e-3
  %33 = sitofp i32 125 to double
  %34 = fadd double %33, %32
  %35 = fmul double %27, %34
  %36 = fadd double %25, %35
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %36)
  ret i32 0
}
