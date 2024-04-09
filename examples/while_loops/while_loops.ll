source_filename = "./examples\while_loops\while_loops.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %i = alloca i32
  store i32 0, i32* %i
  br label %while.cond
while.cond:
  %0 = load i32, i32* %i
  %1 = icmp sle i32 %0, 10
  br i1 %1, label %while.body, label %while.end
while.body:
  %2 = load i32, i32* %i
  %3 = srem i32 %2, 15
  %4 = icmp eq i32 %3, 0
  br i1 %4, label %if.then, label %if.else
if.then:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 150)
  br label %if.end
if.else:
  %6 = load i32, i32* %i
  %7 = srem i32 %6, 5
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %if.then1, label %if.else1
if.then1:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 500)
  br label %if.end1
if.else1:
  %10 = load i32, i32* %i
  %11 = srem i32 %10, 3
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %if.then2, label %if.else2
if.then2:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 300)
  br label %if.end2
if.else2:
  %14 = load i32, i32* %i
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %14)
  br label %if.end2
if.end2:
  br label %if.end1
if.end1:
  br label %if.end
if.end:
  %16 = load i32, i32* %i
  %17 = add i32 %16, 1
  store i32 %17, i32* %i
  br label %while.cond
while.end:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 10000)
  %j = alloca i32
  store i32 0, i32* %j
  %limit = alloca i32
  store i32 5, i32* %limit
  br label %while.cond1
while.cond1:
  %19 = load i32, i32* %j
  %20 = load i32, i32* %limit
  %21 = icmp sle i32 %19, %20
  br i1 %21, label %while.body1, label %while.end1
while.body1:
  %22 = load i32, i32* %j
  %k.2 = alloca i32
  store i32 %22, i32* %k.2
  br label %while.cond2
while.cond2:
  %23 = load i32, i32* %k.2
  %24 = load i32, i32* %limit
  %25 = icmp sle i32 %23, %24
  br i1 %25, label %while.body2, label %while.end2
while.body2:
  %26 = load i32, i32* %k.2
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %26)
  %28 = load i32, i32* %k.2
  %29 = add i32 %28, 1
  store i32 %29, i32* %k.2
  br label %while.cond2
while.end2:
  %30 = load i32, i32* %j
  %31 = add i32 %30, 1
  store i32 %31, i32* %j
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 10000)
  br label %while.cond1
while.end1:
  ret i32 0
}
