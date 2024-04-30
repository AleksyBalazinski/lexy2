source_filename = "./examples\comparisons\comparisons.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define dso_local i32 @main() #0 {
  %x = alloca i32
  store i32 3, i32* %x
  %y = alloca i32
  store i32 5, i32* %y
  %1 = load i32, i32* %x
  %2 = load i32, i32* %y
  %3 = icmp slt i32 %1, %2
  %4 = zext i1 %3 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %4)
  %6 = load i32, i32* %x
  %7 = load i32, i32* %y
  %8 = icmp sgt i32 %6, %7
  %9 = zext i1 %8 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %9)
  %11 = load i32, i32* %x
  %12 = load i32, i32* %y
  %13 = icmp sle i32 %11, %12
  %14 = zext i1 %13 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %14)
  %16 = load i32, i32* %x
  %17 = load i32, i32* %y
  %18 = icmp sge i32 %16, %17
  %19 = zext i1 %18 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %19)
  %21 = load i32, i32* %x
  %22 = load i32, i32* %y
  %23 = icmp eq i32 %21, %22
  %24 = zext i1 %23 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %24)
  %26 = load i32, i32* %x
  %27 = load i32, i32* %y
  %28 = icmp ne i32 %26, %27
  %29 = zext i1 %28 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %29)
  %a = alloca double
  store double 3.14, double* %a
  %b = alloca double
  store double 2.71, double* %b
  %31 = load double, double* %a
  %32 = load double, double* %b
  %33 = fcmp olt double %31, %32
  %34 = zext i1 %33 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %34)
  %36 = load double, double* %a
  %37 = load double, double* %b
  %38 = fcmp ogt double %36, %37
  %39 = zext i1 %38 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %39)
  %41 = load double, double* %a
  %42 = load double, double* %b
  %43 = fcmp ole double %41, %42
  %44 = zext i1 %43 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %44)
  %46 = load double, double* %a
  %47 = load double, double* %b
  %48 = fcmp oge double %46, %47
  %49 = zext i1 %48 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %49)
  %51 = load double, double* %a
  %52 = load double, double* %b
  %53 = fcmp oeq double %51, %52
  %54 = zext i1 %53 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %54)
  %56 = load double, double* %a
  %57 = load double, double* %b
  %58 = fcmp one double %56, %57
  %59 = zext i1 %58 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %59)
  ret i32 0
}
