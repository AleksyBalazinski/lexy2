source_filename = "./examples\scopes\scopes.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %x = alloca double
  store double 3.0, double* %x
  %y = alloca i32
  store i32 2, i32* %y
  %z = alloca double
  store double 3.14, double* %z
  %w = alloca i32
  store i32 1, i32* %w
  %0 = sub i32 0, 1
  %a = alloca i32
  store i32 %0, i32* %a
  %1 = load double, double* %x
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %1)
  %3 = load i32, i32* %y
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %3)
  %5 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %5)
  %7 = load i32, i32* %w
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %7)
  %x.1 = alloca double
  store double 4.0, double* %x.1
  %9 = sitofp i32 10 to double
  %10 = load double, double* %x.1
  %11 = fmul double %10, %9
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
  %18 = sitofp i32 100 to double
  %19 = fmul double %17, %18
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %19)
  %21 = load i32, i32* %y.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %21)
  %23 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %23)
  %25 = load double, double* %w.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %25)
  %27 = load double, double* %w.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %27)
  %y.1.2 = alloca double
  store double 250.0, double* %y.1.2
  %w.1.2 = alloca i32
  store i32 4321, i32* %w.1.2
  %29 = load i32, i32* %w.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %29)
  %31 = load double, double* %y.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %31)
  %33 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %33)
  %35 = load double, double* %y.1.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %35)
  %37 = load double, double* %x
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %37)
  %39 = load i32, i32* %y
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %39)
  %41 = load double, double* %z
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %41)
  %43 = load i32, i32* %w
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %43)
  %x.2 = alloca double
  store double 123.456, double* %x.2
  %45 = load double, double* %x.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %45)
  %a.2.1 = alloca double
  store double 2.25, double* %a.2.1
  %47 = load double, double* %a.2.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %47)
  %a.2.1.1.1 = alloca double
  store double 3.25, double* %a.2.1.1.1
  %49 = load double, double* %a.2.1.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %49)
  %51 = load double, double* %a.2.1.1.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %51)
  %53 = load double, double* %a.2.1
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %53)
  %55 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %55)
  %57 = load i32, i32* %a
  %58 = mul i32 %57, 666
  store i32 %58, i32* %a
  %59 = fsub double 0.0, 1.23
  %a.2.2 = alloca double
  store double %59, double* %a.2.2
  %60 = load double, double* %a.2.2
  call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef %60)
  %62 = sub i32 0, 34
  %a.2.2.2 = alloca i32
  store i32 %62, i32* %a.2.2.2
  %63 = load i32, i32* %a.2.2.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %63)
  %65 = sub i32 0, 35
  %a.2.2.3 = alloca i32
  store i32 %65, i32* %a.2.2.3
  %66 = load i32, i32* %a.2.2.3
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %66)
  %68 = sub i32 0, 36
  %a.2.2.4 = alloca i32
  store i32 %68, i32* %a.2.2.4
  %69 = load i32, i32* %a.2.2.4
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %69)
  %71 = load i32, i32* %a
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %71)
  ret i32 0
}
