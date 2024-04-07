source_filename = "./examples\boolean_variables\boolean_variables.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
  %1 = zext i1 false to i8
  %x = alloca i8
  store i8 %1, i8* %x
  %2 = icmp sgt i32 3, 2
  %3 = zext i1 %2 to i8
  store i8 %3, i8* %x
  %4 = load i8, i8* %x
  %5 = trunc i8 %4 to i1
  %6 = zext i1 %5 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %6)
  %8 = icmp eq i32 100, 100
  %9 = zext i1 %8 to i8
  %y.1 = alloca i8
  store i8 %9, i8* %y.1
  %10 = load i8, i8* %y.1
  %11 = trunc i8 %10 to i1
  %12 = zext i1 %11 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %12)
  %14 = zext i1 false to i8
  %x.1 = alloca i8
  store i8 %14, i8* %x.1
  %15 = load i8, i8* %x.1
  %16 = trunc i8 %15 to i1
  %17 = zext i1 %16 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %17)
  %19 = load i8, i8* %x
  %20 = trunc i8 %19 to i1
  %21 = zext i1 %20 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %21)
  ret i32 0
}
