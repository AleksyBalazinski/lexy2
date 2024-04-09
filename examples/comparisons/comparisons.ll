source_filename = "./examples\comparisons\comparisons.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %x = alloca i32
  store i32 3, i32* %x
  %y = alloca i32
  store i32 5, i32* %y
  %0 = load i32, i32* %x
  %1 = load i32, i32* %y
  %2 = icmp slt i32 %0, %1
  %3 = zext i1 %2 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %3)
  %5 = load i32, i32* %x
  %6 = load i32, i32* %y
  %7 = icmp sgt i32 %5, %6
  %8 = zext i1 %7 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %8)
  %10 = load i32, i32* %x
  %11 = load i32, i32* %y
  %12 = icmp sle i32 %10, %11
  %13 = zext i1 %12 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %13)
  %15 = load i32, i32* %x
  %16 = load i32, i32* %y
  %17 = icmp sge i32 %15, %16
  %18 = zext i1 %17 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %18)
  %20 = load i32, i32* %x
  %21 = load i32, i32* %y
  %22 = icmp eq i32 %20, %21
  %23 = zext i1 %22 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %23)
  %25 = load i32, i32* %x
  %26 = load i32, i32* %y
  %27 = icmp ne i32 %25, %26
  %28 = zext i1 %27 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %28)
  %a = alloca double
  store double 3.14, double* %a
  %b = alloca double
  store double 2.71, double* %b
  %30 = load double, double* %a
  %31 = load double, double* %b
  %32 = fcmp olt double %30, %31
  %33 = zext i1 %32 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %33)
  %35 = load double, double* %a
  %36 = load double, double* %b
  %37 = fcmp ogt double %35, %36
  %38 = zext i1 %37 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %38)
  %40 = load double, double* %a
  %41 = load double, double* %b
  %42 = fcmp ole double %40, %41
  %43 = zext i1 %42 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %43)
  %45 = load double, double* %a
  %46 = load double, double* %b
  %47 = fcmp oge double %45, %46
  %48 = zext i1 %47 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %48)
  %50 = load double, double* %a
  %51 = load double, double* %b
  %52 = fcmp oeq double %50, %51
  %53 = zext i1 %52 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %53)
  %55 = load double, double* %a
  %56 = load double, double* %b
  %57 = fcmp one double %55, %56
  %58 = zext i1 %57 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %58)
  ret i32 0
}
