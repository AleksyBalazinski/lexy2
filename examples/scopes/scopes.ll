source_filename = "./examples\scopes\scopes.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
  %x = alloca double
  store double 3.0, double* %x
  %y = alloca i32
  store i32 2, i32* %y
  %z = alloca double
  store double 3.14, double* %z
  %w = alloca i32
  store i32 1, i32* %w
  %1 = sub i32 0, 1
  %a = alloca i32
  store i32 %1, i32* %a
  %2 = load double, double* %x
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %2)
  %4 = load i32, i32* %y
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %4)
  %6 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %6)
  %8 = load i32, i32* %w
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %8)
  %x.1 = alloca double
  store double 4.0, double* %x.1
  %10 = sitofp i32 10 to double
  %11 = load double, double* %x.1
  %12 = fmul double %11, %10
  store double %12, double* %x.1
  %13 = load double, double* %x.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %13)
  %15 = load i32, i32* %y
  %16 = mul i32 %15, 4
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %16)
  %w.1 = alloca double
  store double 1.23, double* %w.1
  %y.1.1 = alloca i32
  store i32 100, i32* %y.1.1
  %18 = load double, double* %x.1
  %19 = sitofp i32 100 to double
  %20 = fmul double %18, %19
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %20)
  %22 = load i32, i32* %y.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %22)
  %24 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %24)
  %26 = load double, double* %w.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %26)
  %28 = load double, double* %w.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %28)
  %y.1.2 = alloca double
  store double 250.0, double* %y.1.2
  %w.1.2 = alloca i32
  store i32 4321, i32* %w.1.2
  %30 = load i32, i32* %w.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %30)
  %32 = load double, double* %y.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %32)
  %34 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %34)
  %36 = load double, double* %y.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %36)
  %38 = load double, double* %x
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %38)
  %40 = load i32, i32* %y
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %40)
  %42 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %42)
  %44 = load i32, i32* %w
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %44)
  %x.2 = alloca double
  store double 123.456, double* %x.2
  %46 = load double, double* %x.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %46)
  %a.2.1 = alloca double
  store double 2.25, double* %a.2.1
  %48 = load double, double* %a.2.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %48)
  %a.2.1.1.1 = alloca double
  store double 3.25, double* %a.2.1.1.1
  %50 = load double, double* %a.2.1.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %50)
  %52 = load double, double* %a.2.1.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %52)
  %54 = load double, double* %a.2.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %54)
  %56 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %56)
  %58 = load i32, i32* %a
  %59 = mul i32 %58, 666
  store i32 %59, i32* %a
  %60 = fsub double 0.0, 1.23
  %a.2.2 = alloca double
  store double %60, double* %a.2.2
  %61 = load double, double* %a.2.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %61)
  %63 = sub i32 0, 34
  %a.2.2.2 = alloca i32
  store i32 %63, i32* %a.2.2.2
  %64 = load i32, i32* %a.2.2.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %64)
  %66 = sub i32 0, 35
  %a.2.2.3 = alloca i32
  store i32 %66, i32* %a.2.2.3
  %67 = load i32, i32* %a.2.2.3
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %67)
  %69 = sub i32 0, 36
  %a.2.2.4 = alloca i32
  store i32 %69, i32* %a.2.2.4
  %70 = load i32, i32* %a.2.2.4
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %70)
  %72 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %72)
  ret i32 0
}
