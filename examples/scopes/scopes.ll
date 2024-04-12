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
  %10 = load double, double* %x.1
  %11 = fmul double %10, 10.0
  store double %11, double* %x.1
  %12 = load double, double* %x.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %12)
  %14 = load i32, i32* %y
  %15 = mul i32 %14, 4
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %15)
  %w.1 = alloca double
  store double 1.23, double* %w.1
  %y.1.1 = alloca i32
  store i32 100, i32* %y.1.1
  %17 = load double, double* %x.1
  %18 = fmul double %17, 100.0
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %18)
  %20 = load i32, i32* %y.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %20)
  %22 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %22)
  %24 = load double, double* %w.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %24)
  %26 = load double, double* %w.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %26)
  %y.1.2 = alloca double
  store double 250.0, double* %y.1.2
  %w.1.2 = alloca i32
  store i32 4321, i32* %w.1.2
  %28 = load i32, i32* %w.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %28)
  %30 = load double, double* %y.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %30)
  %32 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %32)
  %34 = load double, double* %y.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %34)
  %36 = load double, double* %x
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %36)
  %38 = load i32, i32* %y
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %38)
  %40 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %40)
  %42 = load i32, i32* %w
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %42)
  %x.2 = alloca double
  store double 123.456, double* %x.2
  %44 = load double, double* %x.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %44)
  %a.2.1 = alloca double
  store double 2.25, double* %a.2.1
  %46 = load double, double* %a.2.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %46)
  %a.2.1.1.1 = alloca double
  store double 3.25, double* %a.2.1.1.1
  %48 = load double, double* %a.2.1.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %48)
  %50 = load double, double* %a.2.1.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %50)
  %52 = load double, double* %a.2.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %52)
  %54 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %54)
  %56 = load i32, i32* %a
  %57 = mul i32 %56, 666
  store i32 %57, i32* %a
  %58 = fsub double 0.0, 1.23
  %a.2.2 = alloca double
  store double %58, double* %a.2.2
  %59 = load double, double* %a.2.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %59)
  %61 = sub i32 0, 34
  %a.2.2.2 = alloca i32
  store i32 %61, i32* %a.2.2.2
  %62 = load i32, i32* %a.2.2.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %62)
  %64 = sub i32 0, 35
  %a.2.2.3 = alloca i32
  store i32 %64, i32* %a.2.2.3
  %65 = load i32, i32* %a.2.2.3
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %65)
  %67 = sub i32 0, 36
  %a.2.2.4 = alloca i32
  store i32 %67, i32* %a.2.2.4
  %68 = load i32, i32* %a.2.2.4
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %68)
  %70 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %70)
  ret i32 0
}
