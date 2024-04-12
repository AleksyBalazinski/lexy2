source_filename = "./examples\assignment\assignment.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
  %x = alloca i32
  store i32 3, i32* %x
  %y = alloca i32
  store i32 5, i32* %y
  %1 = load i32, i32* %x
  store i32 %1, i32* %y
  %2 = load i32, i32* %x
  %3 = add i32 %2, %1
  %z = alloca i32
  store i32 %3, i32* %z
  %4 = load i32, i32* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %4)
  %6 = load i32, i32* %x
  %7 = load i32, i32* %z
  %8 = add i32 %7, %6
  store i32 %8, i32* %z
  %9 = load i32, i32* %y
  %10 = add i32 %9, %8
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %10)
  store i32 3, i32* %z
  %12 = load i32, i32* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %12)
  store i32 15, i32* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 15)
  %a = alloca i32
  store i32 3, i32* %a
  %15 = load i32, i32* %a
  %16 = mul i32 %15, 5
  store i32 %16, i32* %a
  %17 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %17)
  %b = alloca double
  store double 13.2, double* %b
  %19 = load double, double* %b
  %20 = fdiv double %19, 4.0
  store double %20, double* %b
  %21 = load double, double* %b
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %21)
  %c = alloca i32
  store i32 5, i32* %c
  %23 = load i32, i32* %c
  %24 = srem i32 %23, 3
  store i32 %24, i32* %c
  %25 = load i32, i32* %c
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %25)
  ret i32 0
}
