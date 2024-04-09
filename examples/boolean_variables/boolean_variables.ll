source_filename = "./examples\boolean_variables\boolean_variables.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %0 = zext i1 false to i8
  %x = alloca i8
  store i8 %0, i8* %x
  %1 = icmp sgt i32 3, 2
  %2 = zext i1 %1 to i8
  store i8 %2, i8* %x
  %3 = load i8, i8* %x
  %4 = trunc i8 %3 to i1
  %5 = zext i1 %4 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %5)
  %7 = icmp eq i32 100, 100
  %8 = zext i1 %7 to i8
  %y.1 = alloca i8
  store i8 %8, i8* %y.1
  %9 = load i8, i8* %y.1
  %10 = trunc i8 %9 to i1
  %11 = zext i1 %10 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %11)
  %13 = zext i1 false to i8
  %x.1 = alloca i8
  store i8 %13, i8* %x.1
  %14 = load i8, i8* %x.1
  %15 = trunc i8 %14 to i1
  %16 = zext i1 %15 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %16)
  %18 = load i8, i8* %x
  %19 = trunc i8 %18 to i1
  %20 = zext i1 %19 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %20)
  ret i32 0
}
