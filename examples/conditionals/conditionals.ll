source_filename = "./examples\conditionals\conditionals.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define dso_local i32 @main() #0 {
  %a = alloca i32
  store i32 0, i32* %a
  %1 = load i32, i32* %a
  %2 = icmp eq i32 %1, 1
  br i1 %2, label %if.then, label %if.else
if.then:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 0)
  br label %if.end
if.else:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 11)
  %b.2 = alloca i32
  store i32 10, i32* %b.2
  %5 = load i32, i32* %b.2
  %6 = icmp slt i32 %5, 100
  br i1 %6, label %if.then1, label %if.end1
if.then1:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 100)
  store i32 2, i32* %b.2
  %8 = sub i32 0, 100
  %b.2.1 = alloca i32
  store i32 %8, i32* %b.2.1
  %9 = load i32, i32* %b.2.1
  %10 = add i32 %9, 1
  %11 = icmp sgt i32 %10, 0
  br i1 %11, label %if.then2, label %if.else1
if.then2:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 99)
  br label %if.end2
if.else1:
  %13 = sub i32 0, 99
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %13)
  %15 = sub i32 0, 1
  %16 = mul i32 %15, 100
  %17 = load i32, i32* %b.2.1
  %18 = icmp eq i32 %17, %16
  br i1 %18, label %if.then3, label %if.else2
if.then3:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 1)
  br label %if.end3
if.else2:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 0)
  br label %if.end3
if.end3:
  br label %if.end2
if.end2:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 69)
  br label %if.end1
if.end1:
  %22 = load i32, i32* %b.2
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %22)
  br label %if.end
if.end:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 13)
  ret i32 0
}
