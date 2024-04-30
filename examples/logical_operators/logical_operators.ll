source_filename = "./examples\logical_operators\logical_operators.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define dso_local i32 @main() #0 {
  %1 = zext i1 false to i8
  %a = alloca i8
  store i8 %1, i8* %a
  %2 = zext i1 true to i8
  %b = alloca i8
  store i8 %2, i8* %b
  %3 = zext i1 false to i8
  %c = alloca i8
  store i8 %3, i8* %c
  %4 = zext i1 true to i8
  %d = alloca i8
  store i8 %4, i8* %d
  %5 = load i8, i8* %b
  %6 = trunc i8 %5 to i1
  %7 = load i8, i8* %c
  %8 = trunc i8 %7 to i1
  %9 = and i1 %6, %8
  %10 = load i8, i8* %d
  %11 = trunc i8 %10 to i1
  %12 = xor i1 %9, %11
  %13 = load i8, i8* %a
  %14 = trunc i8 %13 to i1
  %15 = or i1 %14, %12
  %16 = zext i1 %15 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %16)
  %18 = load i8, i8* %a
  %19 = trunc i8 %18 to i1
  %20 = xor i1 %19, true
  %21 = load i8, i8* %b
  %22 = trunc i8 %21 to i1
  %23 = load i8, i8* %c
  %24 = trunc i8 %23 to i1
  %25 = and i1 %22, %24
  %26 = load i8, i8* %d
  %27 = trunc i8 %26 to i1
  %28 = xor i1 %27, true
  %29 = xor i1 %25, %28
  %30 = or i1 %20, %29
  %31 = zext i1 %30 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %31)
  ret i32 0
}
