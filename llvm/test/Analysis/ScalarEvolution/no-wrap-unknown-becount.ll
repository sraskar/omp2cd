; RUN: opt -analyze -scalar-evolution < %s | FileCheck %s

declare void @llvm.experimental.guard(i1, ...)
declare void @llvm.assume(i1)

define void @s_0(i32 %n, i1* %cond) {
; CHECK-LABEL: Classifying expressions for: @s_0
entry:
  br label %loop

loop:
  %iv = phi i32 [ 0, %entry ], [ %iv.inc, %loop ]
  %iv.inc = add i32 %iv, 1
  %iv.sext = sext i32 %iv to i64
; CHECK:    %iv.sext = sext i32 %iv to i64
; CHECK-NEXT:  -->  {0,+,1}<nuw><nsw><%loop>
  %cmp = icmp slt i32 %iv, %n
  call void(i1, ...) @llvm.experimental.guard(i1 %cmp) [ "deopt"() ]
  %c = load volatile i1, i1* %cond
  br i1 %c, label %loop, label %leave

leave:
  ret void
}

define void @s_1(i1* %cond) {
; CHECK-LABEL: Classifying expressions for: @s_1
entry:
  br label %loop

loop:
  %iv = phi i32 [ 0, %entry ], [ %iv.inc, %loop ]
  %iv.inc = add i32 %iv, 3
  %iv.sext = sext i32 %iv to i64
; CHECK:  %iv.sext = sext i32 %iv to i64
; CHECK-NEXT:  -->  {0,+,3}<nuw><nsw><%loop>
  %cmp = icmp slt i32 %iv, 10000
  call void(i1, ...) @llvm.experimental.guard(i1 %cmp) [ "deopt"() ]
  %c = load volatile i1, i1* %cond
  br i1 %c, label %loop, label %leave

leave:
  ret void
}

define void @s_2(i1* %cond) {
; CHECK-LABEL: Classifying expressions for: @s_2
entry:
  br label %loop

loop:
  %iv = phi i32 [ 0, %entry ], [ %iv.inc, %loop ]
  %iv.inc = add i32 %iv, 3
  %iv.sext = sext i32 %iv to i64
  %cmp = icmp slt i32 %iv, 10000
; CHECK:  %iv.sext = sext i32 %iv to i64
; CHECK-NEXT:  -->  {0,+,3}<nuw><nsw><%loop>
  call void @llvm.assume(i1 %cmp)
  %c = load volatile i1, i1* %cond
  br i1 %c, label %loop, label %leave

leave:
  ret void
}
