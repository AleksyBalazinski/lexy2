source_filename = "./examples\structs\structs.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

%struct.Dog = type { i32, double }
%struct.Car = type { double, double }

define dso_local i32 @main() #0 {
  %doggy = alloca %struct.Dog
  %structIdx = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 0
  store i32 10, i32* %structIdx
  %structIdx1 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 1
  store double 12.34, double* %structIdx1
  %structIdx2 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 0
  %1 = load i32, i32* %structIdx2
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %1)
  %structIdx3 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 1
  %3 = load double, double* %structIdx3
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %3)
  %mustang = alloca %struct.Car
  %structIdx4 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 0
  store double 3342.34, double* %structIdx4
  %structIdx5 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 1
  store double 215.2, double* %structIdx5
  %structIdx6 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 0
  %5 = load double, double* %structIdx6
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %5)
  %structIdx7 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 1
  %7 = load double, double* %structIdx7
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %7)
  ret i32 0
}
