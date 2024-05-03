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

define dso_local i32 @setDogAge(%struct.Dog* noundef %dog, i32 noundef %x) #0 {
  %dog.1 = alloca %struct.Dog*
  store %struct.Dog* %dog, %struct.Dog** %dog.1
  %x.1 = alloca i32
  store i32 %x, i32* %x.1
  %retVal = alloca i32
  %1 = load %struct.Dog*, %struct.Dog** %dog.1
  %structIdx = getelementptr inbounds %struct.Dog, ptr %1, i32 0, i32 0
  %2 = load i32, i32* %x.1
  store i32 %2, i32* %structIdx
  store i32 0, i32* %retVal
  %3 = load i32, i32* %retVal
  ret i32 %3
}

define dso_local i32 @main() #0 {
  %doggy = alloca %struct.Dog
  %structIdx1 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 0
  store i32 10, i32* %structIdx1
  %structIdx2 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 1
  store double 12.34, double* %structIdx2
  %structIdx3 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 0
  %1 = load i32, i32* %structIdx3
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %1)
  %structIdx4 = getelementptr inbounds %struct.Dog, ptr %doggy, i32 0, i32 1
  %3 = load double, double* %structIdx4
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %3)
  %mustang = alloca %struct.Car
  %structIdx5 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 0
  store double 3342.34, double* %structIdx5
  %structIdx6 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 1
  store double 215.2, double* %structIdx6
  %structIdx7 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 0
  %5 = load double, double* %structIdx7
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %5)
  %structIdx8 = getelementptr inbounds %struct.Car, ptr %mustang, i32 0, i32 1
  %7 = load double, double* %structIdx8
  call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double noundef %7)
  %doggo = alloca %struct.Dog
  %structIdx9 = getelementptr inbounds %struct.Dog, ptr %doggo, i32 0, i32 0
  store i32 13, i32* %structIdx9
  %structIdx10 = getelementptr inbounds %struct.Dog, ptr %doggo, i32 0, i32 1
  store double 123.34, double* %structIdx10
  %call = call i32 @setDogAge(%struct.Dog* noundef %doggo, i32 noundef 150)
  %structIdx11 = getelementptr inbounds %struct.Dog, ptr %doggo, i32 0, i32 0
  %9 = load i32, i32* %structIdx11
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %9)
  ret i32 0
}
