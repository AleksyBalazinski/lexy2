source_filename = "./examples\conditionals\conditionals.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %a = alloca i32
  store i32 0, i32* %a
  %0 = load i32, i32* %a
  %1 = icmp eq i32 %0, 1
  br i1 %1, label %if.then, label %if.else
if.then:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 0)
  br label %if.end
if.else:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 11)
  %b.2 = alloca i32
  store i32 10, i32* %b.2
  %4 = load i32, i32* %b.2
  %5 = icmp slt i32 %4, 100
  br i1 %5, label %if.then1, label %if.end1
if.then1:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 100)
  store i32 2, i32* %b.2
  %7 = sub i32 0, 100
  %b.2.1 = alloca i32
  store i32 %7, i32* %b.2.1
  %8 = load i32, i32* %b.2.1
  %9 = add i32 %8, 1
  %10 = icmp sgt i32 %9, 0
  br i1 %10, label %if.then2, label %if.else1
if.then2:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 99)
  br label %if.end2
if.else1:
  %12 = sub i32 0, 99
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %12)
  %14 = sub i32 0, 1
  %15 = mul i32 %14, 100
  %16 = load i32, i32* %b.2.1
  %17 = icmp eq i32 %16, %15
  br i1 %17, label %if.then3, label %if.else2
if.then3:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 1)
  br label %if.end3
if.else2:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 0)
  br label %if.end3
if.end3:
  br label %if.end2
if.end2:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 69)
  br label %if.end1
if.end1:
  %21 = load i32, i32* %b.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %21)
  br label %if.end
if.end:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 13)
  ret i32 0
}
