source_filename = "./examples\while_loops\while_loops.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
  %i = alloca i32
  store i32 0, i32* %i
  br label %while.cond
while.cond:
  %1 = load i32, i32* %i
  %2 = icmp sle i32 %1, 10
  br i1 %2, label %while.body, label %while.end
while.body:
  %3 = load i32, i32* %i
  %4 = srem i32 %3, 15
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %if.then, label %if.else
if.then:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 150)
  br label %if.end
if.else:
  %7 = load i32, i32* %i
  %8 = srem i32 %7, 5
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %if.then1, label %if.else1
if.then1:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 500)
  br label %if.end1
if.else1:
  %11 = load i32, i32* %i
  %12 = srem i32 %11, 3
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %if.then2, label %if.else2
if.then2:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 300)
  br label %if.end2
if.else2:
  %15 = load i32, i32* %i
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %15)
  br label %if.end2
if.end2:
  br label %if.end1
if.end1:
  br label %if.end
if.end:
  %17 = load i32, i32* %i
  %18 = add i32 %17, 1
  store i32 %18, i32* %i
  br label %while.cond
while.end:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 10000)
  %j = alloca i32
  store i32 0, i32* %j
  %limit = alloca i32
  store i32 5, i32* %limit
  br label %while.cond1
while.cond1:
  %20 = load i32, i32* %j
  %21 = load i32, i32* %limit
  %22 = icmp sle i32 %20, %21
  br i1 %22, label %while.body1, label %while.end1
while.body1:
  %23 = load i32, i32* %j
  %k.2 = alloca i32
  store i32 %23, i32* %k.2
  br label %while.cond2
while.cond2:
  %24 = load i32, i32* %k.2
  %25 = load i32, i32* %limit
  %26 = icmp sle i32 %24, %25
  br i1 %26, label %while.body2, label %while.end2
while.body2:
  %27 = load i32, i32* %k.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %27)
  %29 = load i32, i32* %k.2
  %30 = add i32 %29, 1
  store i32 %30, i32* %k.2
  br label %while.cond2
while.end2:
  %31 = load i32, i32* %j
  %32 = add i32 %31, 1
  store i32 %32, i32* %j
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 10000)
  br label %while.cond1
while.end1:
  ret i32 0
}
