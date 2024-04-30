source_filename = "./examples\arrays\arrays.l2"
target triple = "x86_64-w64-windows-gnu"

@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define dso_local i32 @main() #0 {
  %m1 = alloca [2 x [2 x i32]]
  %arrayIdx = getelementptr inbounds [2 x [2 x i32]], ptr %m1, i64 0, i64 0
  %arrayIdx1 = getelementptr inbounds [2 x i32], ptr %arrayIdx, i64 0, i64 0
  store i32 1, i32* %arrayIdx1
  %arrayIdx2 = getelementptr inbounds [2 x [2 x i32]], ptr %m1, i64 0, i64 0
  %arrayIdx3 = getelementptr inbounds [2 x i32], ptr %arrayIdx2, i64 0, i64 1
  store i32 2, i32* %arrayIdx3
  %arrayIdx4 = getelementptr inbounds [2 x [2 x i32]], ptr %m1, i64 0, i64 1
  %arrayIdx5 = getelementptr inbounds [2 x i32], ptr %arrayIdx4, i64 0, i64 0
  store i32 3, i32* %arrayIdx5
  %arrayIdx6 = getelementptr inbounds [2 x [2 x i32]], ptr %m1, i64 0, i64 1
  %arrayIdx7 = getelementptr inbounds [2 x i32], ptr %arrayIdx6, i64 0, i64 1
  store i32 4, i32* %arrayIdx7
  %m2 = alloca [2 x [3 x i32]]
  %arrayIdx8 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 0
  %arrayIdx9 = getelementptr inbounds [3 x i32], ptr %arrayIdx8, i64 0, i64 0
  store i32 1, i32* %arrayIdx9
  %arrayIdx10 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 0
  %arrayIdx11 = getelementptr inbounds [3 x i32], ptr %arrayIdx10, i64 0, i64 1
  store i32 7, i32* %arrayIdx11
  %arrayIdx12 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 0
  %arrayIdx13 = getelementptr inbounds [3 x i32], ptr %arrayIdx12, i64 0, i64 2
  %1 = sub i32 0, 1
  store i32 %1, i32* %arrayIdx13
  %arrayIdx14 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 1
  %arrayIdx15 = getelementptr inbounds [3 x i32], ptr %arrayIdx14, i64 0, i64 0
  store i32 1, i32* %arrayIdx15
  %arrayIdx16 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 1
  %arrayIdx17 = getelementptr inbounds [3 x i32], ptr %arrayIdx16, i64 0, i64 1
  store i32 3, i32* %arrayIdx17
  %arrayIdx18 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 1
  %arrayIdx19 = getelementptr inbounds [3 x i32], ptr %arrayIdx18, i64 0, i64 2
  %2 = sub i32 0, 2
  store i32 %2, i32* %arrayIdx19
  %result = alloca [2 x [3 x i32]]
  %i = alloca i32
  store i32 0, i32* %i
  br label %while.cond
while.cond:
  %3 = load i32, i32* %i
  %4 = icmp slt i32 %3, 2
  br i1 %4, label %while.body, label %while.end
while.body:
  %j.1 = alloca i32
  store i32 0, i32* %j.1
  br label %while.cond1
while.cond1:
  %5 = load i32, i32* %j.1
  %6 = icmp slt i32 %5, 3
  br i1 %6, label %while.body1, label %while.end1
while.body1:
  %7 = load i32, i32* %i
  %8 = sext i32 %7 to i64
  %arrayIdx20 = getelementptr inbounds [2 x [3 x i32]], ptr %result, i64 0, i64 %8
  %9 = load i32, i32* %j.1
  %10 = sext i32 %9 to i64
  %arrayIdx21 = getelementptr inbounds [3 x i32], ptr %arrayIdx20, i64 0, i64 %10
  store i32 0, i32* %arrayIdx21
  %k.1.1 = alloca i32
  store i32 0, i32* %k.1.1
  br label %while.cond2
while.cond2:
  %11 = load i32, i32* %k.1.1
  %12 = icmp slt i32 %11, 2
  br i1 %12, label %while.body2, label %while.end2
while.body2:
  %13 = load i32, i32* %i
  %14 = sext i32 %13 to i64
  %arrayIdx22 = getelementptr inbounds [2 x [3 x i32]], ptr %result, i64 0, i64 %14
  %15 = load i32, i32* %j.1
  %16 = sext i32 %15 to i64
  %arrayIdx23 = getelementptr inbounds [3 x i32], ptr %arrayIdx22, i64 0, i64 %16
  %17 = load i32, i32* %i
  %18 = sext i32 %17 to i64
  %arrayIdx24 = getelementptr inbounds [2 x [2 x i32]], ptr %m1, i64 0, i64 %18
  %19 = load i32, i32* %k.1.1
  %20 = sext i32 %19 to i64
  %arrayIdx25 = getelementptr inbounds [2 x i32], ptr %arrayIdx24, i64 0, i64 %20
  %21 = load i32, i32* %k.1.1
  %22 = sext i32 %21 to i64
  %arrayIdx26 = getelementptr inbounds [2 x [3 x i32]], ptr %m2, i64 0, i64 %22
  %23 = load i32, i32* %j.1
  %24 = sext i32 %23 to i64
  %arrayIdx27 = getelementptr inbounds [3 x i32], ptr %arrayIdx26, i64 0, i64 %24
  %25 = load i32, i32* %arrayIdx25
  %26 = load i32, i32* %arrayIdx27
  %27 = mul i32 %25, %26
  %28 = load i32, i32* %arrayIdx23
  %29 = add i32 %28, %27
  store i32 %29, i32* %arrayIdx23
  %30 = load i32, i32* %k.1.1
  %31 = add i32 %30, 1
  store i32 %31, i32* %k.1.1
  br label %while.cond2
while.end2:
  %32 = load i32, i32* %i
  %33 = sext i32 %32 to i64
  %arrayIdx28 = getelementptr inbounds [2 x [3 x i32]], ptr %result, i64 0, i64 %33
  %34 = load i32, i32* %j.1
  %35 = sext i32 %34 to i64
  %arrayIdx29 = getelementptr inbounds [3 x i32], ptr %arrayIdx28, i64 0, i64 %35
  %36 = load i32, i32* %arrayIdx29
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %36)
  %38 = load i32, i32* %j.1
  %39 = add i32 %38, 1
  store i32 %39, i32* %j.1
  br label %while.cond1
while.end1:
  %40 = load i32, i32* %i
  %41 = add i32 %40, 1
  store i32 %41, i32* %i
  br label %while.cond
while.end:
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef 1111)
  %prime = alloca [100 x i8]
  store i32 0, i32* %i
  br label %while.cond3
while.cond3:
  %43 = load i32, i32* %i
  %44 = icmp slt i32 %43, 100
  br i1 %44, label %while.body3, label %while.end3
while.body3:
  %45 = load i32, i32* %i
  %46 = sext i32 %45 to i64
  %arrayIdx30 = getelementptr inbounds [100 x i8], ptr %prime, i64 0, i64 %46
  %47 = zext i1 true to i8
  store i8 %47, i8* %arrayIdx30
  %48 = load i32, i32* %i
  %49 = add i32 %48, 1
  store i32 %49, i32* %i
  br label %while.cond3
while.end3:
  %p = alloca i32
  store i32 2, i32* %p
  br label %while.cond4
while.cond4:
  %50 = load i32, i32* %p
  %51 = load i32, i32* %p
  %52 = mul i32 %50, %51
  %53 = icmp sle i32 %52, 100
  br i1 %53, label %while.body4, label %while.end4
while.body4:
  %54 = load i32, i32* %p
  %55 = sext i32 %54 to i64
  %arrayIdx31 = getelementptr inbounds [100 x i8], ptr %prime, i64 0, i64 %55
  %56 = load i8, i8* %arrayIdx31
  %57 = trunc i8 %56 to i1
  br i1 %57, label %if.then, label %if.end
if.then:
  %58 = load i32, i32* %p
  %59 = load i32, i32* %p
  %60 = mul i32 %58, %59
  %l.3.1 = alloca i32
  store i32 %60, i32* %l.3.1
  br label %while.cond5
while.cond5:
  %61 = load i32, i32* %l.3.1
  %62 = icmp slt i32 %61, 100
  br i1 %62, label %while.body5, label %while.end5
while.body5:
  %63 = load i32, i32* %l.3.1
  %64 = sext i32 %63 to i64
  %arrayIdx32 = getelementptr inbounds [100 x i8], ptr %prime, i64 0, i64 %64
  %65 = zext i1 false to i8
  store i8 %65, i8* %arrayIdx32
  %66 = load i32, i32* %p
  %67 = load i32, i32* %l.3.1
  %68 = add i32 %67, %66
  store i32 %68, i32* %l.3.1
  br label %while.cond5
while.end5:
  br label %if.end
if.end:
  %69 = load i32, i32* %p
  %70 = add i32 %69, 1
  store i32 %70, i32* %p
  br label %while.cond4
while.end4:
  store i32 2, i32* %p
  br label %while.cond6
while.cond6:
  %71 = load i32, i32* %p
  %72 = icmp sle i32 %71, 100
  br i1 %72, label %while.body6, label %while.end6
while.body6:
  %73 = load i32, i32* %p
  %74 = sext i32 %73 to i64
  %arrayIdx33 = getelementptr inbounds [100 x i8], ptr %prime, i64 0, i64 %74
  %75 = load i8, i8* %arrayIdx33
  %76 = trunc i8 %75 to i1
  br i1 %76, label %if.then1, label %if.end1
if.then1:
  %77 = load i32, i32* %p
  call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 noundef %77)
  br label %if.end1
if.end1:
  %79 = load i32, i32* %p
  %80 = add i32 %79, 1
  store i32 %80, i32* %p
  br label %while.cond6
while.end6:
  ret i32 0
}
