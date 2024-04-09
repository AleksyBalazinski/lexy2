source_filename = "./examples\assignment\assignment.l2"
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
  store i32 %0, i32* %y
  %1 = load i32, i32* %x
  %2 = add i32 %1, %0
  %z = alloca i32
  store i32 %2, i32* %z
  %3 = load i32, i32* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %3)
  %5 = load i32, i32* %x
  %6 = load i32, i32* %z
  %7 = add i32 %6, %5
  store i32 %7, i32* %z
  %8 = load i32, i32* %y
  %9 = add i32 %8, %7
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %9)
  store i32 3, i32* %z
  %11 = load i32, i32* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %11)
  store i32 15, i32* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 15)
  %a = alloca i32
  store i32 3, i32* %a
  %14 = load i32, i32* %a
  %15 = mul i32 %14, 5
  store i32 %15, i32* %a
  %16 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %16)
  %b = alloca double
  store double 13.2, double* %b
  %18 = sitofp i32 4 to double
  %19 = load double, double* %b
  %20 = fdiv double %19, %18
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
