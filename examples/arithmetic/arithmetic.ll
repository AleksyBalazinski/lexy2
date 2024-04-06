source_filename = "./examples\arithmetic\arithmetic.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

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
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %10)
  %12 = sitofp i32 8 to double
  %13 = fmul double 3.14, %12
  %14 = fptosi double %13 to i32
  %15 = sitofp i32 2 to double
  %16 = sitofp i32 %14 to double
  %17 = fmul double %16, %15
  %18 = fdiv double %17, 3.0
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %18)
  %20 = sitofp i32 3 to double
  %21 = fadd double %20, 3.0
  %22 = fsub double 0.0, 1.12
  %23 = sitofp i32 45 to double
  %24 = fmul double %23, %22
  %25 = fadd double %21, %24
  %26 = fmul double 12.5, %25
  %27 = sitofp i32 128 to double
  %28 = fdiv double %27, 3.0
  %29 = sub i32 0, 45
  %30 = mul i32 %29, 23
  %31 = sitofp i32 %30 to double
  %32 = fadd double %31, 12.123
  %33 = fmul double %32, 1.0e-3
  %34 = sitofp i32 125 to double
  %35 = fadd double %34, %33
  %36 = fmul double %28, %35
  %37 = fadd double %26, %36
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %37)
  ret i32 0
}
