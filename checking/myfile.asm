	.file	"test.cpp"
	.intel_syntax noprefix
	.text
	.section	.text._ZSt23__is_constant_evaluatedv,"axG",@progbits,_ZSt23__is_constant_evaluatedv,comdat
	.weak	_ZSt23__is_constant_evaluatedv
	.type	_ZSt23__is_constant_evaluatedv, @function
_ZSt23__is_constant_evaluatedv:
.LFB1:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	eax, 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_ZSt23__is_constant_evaluatedv, .-_ZSt23__is_constant_evaluatedv
	.section	.text._ZNSt11char_traitsIcE6lengthEPKc,"axG",@progbits,_ZNSt11char_traitsIcE6lengthEPKc,comdat
	.weak	_ZNSt11char_traitsIcE6lengthEPKc
	.type	_ZNSt11char_traitsIcE6lengthEPKc, @function
_ZNSt11char_traitsIcE6lengthEPKc:
.LFB115:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], rdi
	call	_ZSt23__is_constant_evaluatedv
	test	al, al
	je	.L4
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc
	jmp	.L5
.L4:
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	strlen@PLT
	nop
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE115:
	.size	_ZNSt11char_traitsIcE6lengthEPKc, .-_ZNSt11char_traitsIcE6lengthEPKc
	.section	.text._ZStorSt13_Ios_OpenmodeS_,"axG",@progbits,_ZStorSt13_Ios_OpenmodeS_,comdat
	.weak	_ZStorSt13_Ios_OpenmodeS_
	.type	_ZStorSt13_Ios_OpenmodeS_, @function
_ZStorSt13_Ios_OpenmodeS_:
.LFB1582:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -4[rbp], edi
	mov	DWORD PTR -8[rbp], esi
	mov	eax, DWORD PTR -4[rbp]
	or	eax, DWORD PTR -8[rbp]
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1582:
	.size	_ZStorSt13_Ios_OpenmodeS_, .-_ZStorSt13_Ios_OpenmodeS_
#APP
	.globl _ZSt21ios_base_library_initv
	.section	.rodata
.LC0:
	.string	"id.data"
.LC1:
	.string	"name.data"
.LC2:
	.string	"email.data"
.LC3:
	.string	"table.index"
.LC4:
	.string	"Inserted row: id="
.LC5:
	.string	", name="
.LC6:
	.string	", email="
.LC7:
	.string	"\n"
#NO_APP
	.text
	.globl	_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_
	.type	_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_, @function
_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_:
.LFB2119:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2119
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 2184
	.cfi_offset 3, -24
	mov	QWORD PTR -2168[rbp], rdi
	mov	QWORD PTR -2176[rbp], rsi
	mov	QWORD PTR -2184[rbp], rdx
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -24[rbp], rax
	xor	eax, eax
	mov	esi, 1
	mov	edi, 4
	call	_ZStorSt13_Ios_OpenmodeS_
	mov	edx, eax
	lea	rax, -2080[rbp]
	lea	rcx, .LC0[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB0:
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEEC1EPKcSt13_Ios_Openmode@PLT
.LEHE0:
	mov	esi, 1
	mov	edi, 4
	call	_ZStorSt13_Ios_OpenmodeS_
	mov	edx, eax
	lea	rax, -1568[rbp]
	lea	rcx, .LC1[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB1:
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEEC1EPKcSt13_Ios_Openmode@PLT
.LEHE1:
	mov	esi, 1
	mov	edi, 4
	call	_ZStorSt13_Ios_OpenmodeS_
	mov	edx, eax
	lea	rax, -1056[rbp]
	lea	rcx, .LC2[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB2:
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEEC1EPKcSt13_Ios_Openmode@PLT
.LEHE2:
	mov	esi, 1
	mov	edi, 4
	call	_ZStorSt13_Ios_OpenmodeS_
	mov	edx, eax
	lea	rax, -544[rbp]
	lea	rcx, .LC3[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB3:
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEEC1EPKcSt13_Ios_Openmode@PLT
.LEHE3:
	lea	rax, -2080[rbp]
	mov	edx, 2
	mov	esi, 0
	mov	rdi, rax
.LEHB4:
	call	_ZNSo5seekpElSt12_Ios_Seekdir@PLT
	lea	rax, -2080[rbp]
	mov	rdi, rax
	call	_ZNSo5tellpEv@PLT
	mov	QWORD PTR -2096[rbp], rax
	mov	QWORD PTR -2088[rbp], rdx
	lea	rax, -2096[rbp]
	mov	rdi, rax
	call	_ZNKSt4fposI11__mbstate_tEcvlEv
	mov	QWORD PTR -2160[rbp], rax
	lea	rax, -1568[rbp]
	mov	edx, 2
	mov	esi, 0
	mov	rdi, rax
	call	_ZNSo5seekpElSt12_Ios_Seekdir@PLT
	lea	rax, -1568[rbp]
	mov	rdi, rax
	call	_ZNSo5tellpEv@PLT
	mov	QWORD PTR -2096[rbp], rax
	mov	QWORD PTR -2088[rbp], rdx
	lea	rax, -2096[rbp]
	mov	rdi, rax
	call	_ZNKSt4fposI11__mbstate_tEcvlEv
	mov	QWORD PTR -2152[rbp], rax
	lea	rax, -1056[rbp]
	mov	edx, 2
	mov	esi, 0
	mov	rdi, rax
	call	_ZNSo5seekpElSt12_Ios_Seekdir@PLT
	lea	rax, -1056[rbp]
	mov	rdi, rax
	call	_ZNSo5tellpEv@PLT
	mov	QWORD PTR -2096[rbp], rax
	mov	QWORD PTR -2088[rbp], rdx
	lea	rax, -2096[rbp]
	mov	rdi, rax
	call	_ZNKSt4fposI11__mbstate_tEcvlEv
	mov	QWORD PTR -2144[rbp], rax
	lea	rcx, -2168[rbp]
	lea	rax, -2080[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	mov	rax, QWORD PTR -2176[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4sizeEv@PLT
	mov	rbx, rax
	mov	rax, QWORD PTR -2176[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4dataEv@PLT
	mov	rcx, rax
	lea	rax, -1568[rbp]
	mov	rdx, rbx
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	mov	rax, QWORD PTR -2184[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4sizeEv@PLT
	mov	rbx, rax
	mov	rax, QWORD PTR -2184[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4dataEv@PLT
	mov	rcx, rax
	lea	rax, -1056[rbp]
	mov	rdx, rbx
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	mov	rax, QWORD PTR -2160[rbp]
	add	rax, 8
	mov	QWORD PTR -2136[rbp], rax
	mov	rax, QWORD PTR -2176[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4sizeEv@PLT
	mov	rdx, QWORD PTR -2152[rbp]
	add	rax, rdx
	mov	QWORD PTR -2128[rbp], rax
	mov	rax, QWORD PTR -2184[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4sizeEv@PLT
	mov	rdx, QWORD PTR -2144[rbp]
	add	rax, rdx
	mov	QWORD PTR -2120[rbp], rax
	lea	rax, -544[rbp]
	mov	rdi, rax
	call	_ZNSo5tellpEv@PLT
	mov	QWORD PTR -2096[rbp], rax
	mov	QWORD PTR -2088[rbp], rdx
	lea	rax, -2096[rbp]
	mov	rdi, rax
	call	_ZNKSt4fposI11__mbstate_tEcvlEv
	mov	QWORD PTR -2112[rbp], rax
	mov	rax, QWORD PTR -2112[rbp]
	add	rax, 64
	mov	QWORD PTR -2104[rbp], rax
	lea	rcx, -2112[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2104[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2160[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2136[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2152[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2128[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2144[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rcx, -2120[rbp]
	lea	rax, -544[rbp]
	mov	edx, 8
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSo5writeEPKcl@PLT
	lea	rax, .LC4[rip]
	mov	rsi, rax
	lea	rax, _ZSt4cout[rip]
	mov	rdi, rax
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -2168[rbp]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZNSolsEl@PLT
	mov	rdx, rax
	lea	rax, .LC5[rip]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -2176[rbp]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE@PLT
	mov	rdx, rax
	lea	rax, .LC6[rip]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -2184[rbp]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE@PLT
	mov	rdx, rax
	lea	rax, .LC7[rip]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
.LEHE4:
	lea	rax, -544[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	lea	rax, -1056[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	lea	rax, -1568[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	lea	rax, -2080[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	mov	rax, QWORD PTR -24[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L14
	jmp	.L19
.L18:
	endbr64
	mov	rbx, rax
	lea	rax, -544[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	jmp	.L10
.L17:
	endbr64
	mov	rbx, rax
.L10:
	lea	rax, -1056[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	jmp	.L11
.L16:
	endbr64
	mov	rbx, rax
.L11:
	lea	rax, -1568[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	jmp	.L12
.L15:
	endbr64
	mov	rbx, rax
.L12:
	lea	rax, -2080[rbp]
	mov	rdi, rax
	call	_ZNSt14basic_ofstreamIcSt11char_traitsIcEED1Ev@PLT
	mov	rax, rbx
	mov	rdx, QWORD PTR -24[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L13
	call	__stack_chk_fail@PLT
.L13:
	mov	rdi, rax
.LEHB5:
	call	_Unwind_Resume@PLT
.LEHE5:
.L19:
	call	__stack_chk_fail@PLT
.L14:
	mov	rbx, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2119:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA2119:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2119-.LLSDACSB2119
.LLSDACSB2119:
	.uleb128 .LEHB0-.LFB2119
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB2119
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L15-.LFB2119
	.uleb128 0
	.uleb128 .LEHB2-.LFB2119
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L16-.LFB2119
	.uleb128 0
	.uleb128 .LEHB3-.LFB2119
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L17-.LFB2119
	.uleb128 0
	.uleb128 .LEHB4-.LFB2119
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L18-.LFB2119
	.uleb128 0
	.uleb128 .LEHB5-.LFB2119
	.uleb128 .LEHE5-.LEHB5
	.uleb128 0
	.uleb128 0
.LLSDACSE2119:
	.text
	.size	_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_, .-_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_
	.section	.rodata
.LC8:
	.string	"alice@example.com"
.LC9:
	.string	"Alice"
.LC10:
	.string	"bob@shop.com"
.LC11:
	.string	"Bob"
.LC12:
	.string	"carol@example.org"
.LC13:
	.string	"Carol"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2120:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2120
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 152
	.cfi_offset 3, -24
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -24[rbp], rax
	xor	eax, eax
	lea	rax, -145[rbp]
	mov	QWORD PTR -144[rbp], rax
	nop
	nop
	lea	rdx, -145[rbp]
	lea	rax, -64[rbp]
	lea	rcx, .LC8[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB6:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
.LEHE6:
	lea	rax, -146[rbp]
	mov	QWORD PTR -136[rbp], rax
	nop
	nop
	lea	rdx, -146[rbp]
	lea	rax, -96[rbp]
	lea	rcx, .LC9[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB7:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
.LEHE7:
	lea	rdx, -64[rbp]
	lea	rax, -96[rbp]
	mov	rsi, rax
	mov	edi, 10
.LEHB8:
	call	_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_
.LEHE8:
	lea	rax, -96[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	lea	rax, -146[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -64[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	lea	rax, -145[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -145[rbp]
	mov	QWORD PTR -128[rbp], rax
	nop
	nop
	lea	rdx, -145[rbp]
	lea	rax, -64[rbp]
	lea	rcx, .LC10[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB9:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
.LEHE9:
	lea	rax, -146[rbp]
	mov	QWORD PTR -120[rbp], rax
	nop
	nop
	lea	rdx, -146[rbp]
	lea	rax, -96[rbp]
	lea	rcx, .LC11[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB10:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
.LEHE10:
	lea	rdx, -64[rbp]
	lea	rax, -96[rbp]
	mov	rsi, rax
	mov	edi, 20
.LEHB11:
	call	_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_
.LEHE11:
	lea	rax, -96[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	lea	rax, -146[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -64[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	lea	rax, -145[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -145[rbp]
	mov	QWORD PTR -112[rbp], rax
	nop
	nop
	lea	rdx, -145[rbp]
	lea	rax, -64[rbp]
	lea	rcx, .LC12[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB12:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
.LEHE12:
	lea	rax, -146[rbp]
	mov	QWORD PTR -104[rbp], rax
	nop
	nop
	lea	rdx, -146[rbp]
	lea	rax, -96[rbp]
	lea	rcx, .LC13[rip]
	mov	rsi, rcx
	mov	rdi, rax
.LEHB13:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
.LEHE13:
	lea	rdx, -64[rbp]
	lea	rax, -96[rbp]
	mov	rsi, rax
	mov	edi, 30
.LEHB14:
	call	_Z10insert_rowlRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_
.LEHE14:
	lea	rax, -96[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	lea	rax, -146[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -64[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	lea	rax, -145[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	mov	eax, 0
	mov	rdx, QWORD PTR -24[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L34
	jmp	.L44
.L37:
	endbr64
	mov	rbx, rax
	lea	rax, -96[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	jmp	.L23
.L36:
	endbr64
	mov	rbx, rax
.L23:
	lea	rax, -146[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -64[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	jmp	.L24
.L35:
	endbr64
	mov	rbx, rax
.L24:
	lea	rax, -145[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	mov	rax, rbx
	mov	rdx, QWORD PTR -24[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L25
	call	__stack_chk_fail@PLT
.L25:
	mov	rdi, rax
.LEHB15:
	call	_Unwind_Resume@PLT
.L40:
	endbr64
	mov	rbx, rax
	lea	rax, -96[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	jmp	.L27
.L39:
	endbr64
	mov	rbx, rax
.L27:
	lea	rax, -146[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -64[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	jmp	.L28
.L38:
	endbr64
	mov	rbx, rax
.L28:
	lea	rax, -145[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	mov	rax, rbx
	mov	rdx, QWORD PTR -24[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L29
	call	__stack_chk_fail@PLT
.L29:
	mov	rdi, rax
	call	_Unwind_Resume@PLT
.L43:
	endbr64
	mov	rbx, rax
	lea	rax, -96[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	jmp	.L31
.L42:
	endbr64
	mov	rbx, rax
.L31:
	lea	rax, -146[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	lea	rax, -64[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@PLT
	jmp	.L32
.L41:
	endbr64
	mov	rbx, rax
.L32:
	lea	rax, -145[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	mov	rax, rbx
	mov	rdx, QWORD PTR -24[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L33
	call	__stack_chk_fail@PLT
.L33:
	mov	rdi, rax
	call	_Unwind_Resume@PLT
.LEHE15:
.L44:
	call	__stack_chk_fail@PLT
.L34:
	mov	rbx, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2120:
	.section	.gcc_except_table
.LLSDA2120:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2120-.LLSDACSB2120
.LLSDACSB2120:
	.uleb128 .LEHB6-.LFB2120
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L35-.LFB2120
	.uleb128 0
	.uleb128 .LEHB7-.LFB2120
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L36-.LFB2120
	.uleb128 0
	.uleb128 .LEHB8-.LFB2120
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L37-.LFB2120
	.uleb128 0
	.uleb128 .LEHB9-.LFB2120
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L38-.LFB2120
	.uleb128 0
	.uleb128 .LEHB10-.LFB2120
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L39-.LFB2120
	.uleb128 0
	.uleb128 .LEHB11-.LFB2120
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L40-.LFB2120
	.uleb128 0
	.uleb128 .LEHB12-.LFB2120
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L41-.LFB2120
	.uleb128 0
	.uleb128 .LEHB13-.LFB2120
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L42-.LFB2120
	.uleb128 0
	.uleb128 .LEHB14-.LFB2120
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L43-.LFB2120
	.uleb128 0
	.uleb128 .LEHB15-.LFB2120
	.uleb128 .LEHE15-.LEHB15
	.uleb128 0
	.uleb128 0
.LLSDACSE2120:
	.text
	.size	main, .-main
	.section	.text._ZN9__gnu_cxx11char_traitsIcE6lengthEPKc,"axG",@progbits,_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc,comdat
	.align 2
	.weak	_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc
	.type	_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc, @function
_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc:
.LFB2121:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	QWORD PTR -16[rbp], 0
	jmp	.L46
.L47:
	add	QWORD PTR -16[rbp], 1
.L46:
	mov	BYTE PTR -17[rbp], 0
	mov	rdx, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -16[rbp]
	add	rdx, rax
	lea	rax, -17[rbp]
	mov	rsi, rax
	mov	rdi, rdx
	call	_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_
	xor	eax, 1
	test	al, al
	jne	.L47
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR -8[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L49
	call	__stack_chk_fail@PLT
.L49:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2121:
	.size	_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc, .-_ZN9__gnu_cxx11char_traitsIcE6lengthEPKc
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD5Ev,comdat
	.align 2
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev:
.LFB2230:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	_ZNSt15__new_allocatorIcED2Ev
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2230:
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD1Ev
	.set	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD1Ev,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD2Ev
	.section	.text._ZNKSt4fposI11__mbstate_tEcvlEv,"axG",@progbits,_ZNKSt4fposI11__mbstate_tEcvlEv,comdat
	.align 2
	.weak	_ZNKSt4fposI11__mbstate_tEcvlEv
	.type	_ZNKSt4fposI11__mbstate_tEcvlEv, @function
_ZNKSt4fposI11__mbstate_tEcvlEv:
.LFB2398:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -8[rbp], rdi
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR [rax]
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2398:
	.size	_ZNKSt4fposI11__mbstate_tEcvlEv, .-_ZNKSt4fposI11__mbstate_tEcvlEv
	.section	.rodata
	.align 8
.LC14:
	.string	"basic_string: construction from null is not valid"
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC5IS3_EEPKcRKS3_,comdat
	.align 2
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_:
.LFB2404:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2404
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 56
	.cfi_offset 3, -24
	mov	QWORD PTR -40[rbp], rdi
	mov	QWORD PTR -48[rbp], rsi
	mov	QWORD PTR -56[rbp], rdx
	mov	rbx, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_M_local_dataEv@PLT
	mov	rcx, rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rdx, rax
	mov	rsi, rcx
	mov	rdi, rbx
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderC1EPcRKS3_@PLT
	cmp	QWORD PTR -48[rbp], 0
	jne	.L54
	lea	rax, .LC14[rip]
	mov	rdi, rax
.LEHB16:
	call	_ZSt19__throw_logic_errorPKc@PLT
.L54:
	mov	rax, QWORD PTR -48[rbp]
	mov	rdi, rax
	call	_ZNSt11char_traitsIcE6lengthEPKc
	mov	rdx, QWORD PTR -48[rbp]
	add	rax, rdx
	mov	QWORD PTR -24[rbp], rax
	mov	rdx, QWORD PTR -24[rbp]
	mov	rcx, QWORD PTR -48[rbp]
	mov	rax, QWORD PTR -40[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag
.LEHE16:
	jmp	.L57
.L56:
	endbr64
	mov	rbx, rax
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderD1Ev
	mov	rax, rbx
	mov	rdi, rax
.LEHB17:
	call	_Unwind_Resume@PLT
.LEHE17:
.L57:
	mov	rbx, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2404:
	.section	.gcc_except_table
.LLSDA2404:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2404-.LLSDACSB2404
.LLSDACSB2404:
	.uleb128 .LEHB16-.LFB2404
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L56-.LFB2404
	.uleb128 0
	.uleb128 .LEHB17-.LFB2404
	.uleb128 .LEHE17-.LEHB17
	.uleb128 0
	.uleb128 0
.LLSDACSE2404:
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC5IS3_EEPKcRKS3_,comdat
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_
	.set	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1IS3_EEPKcRKS3_,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2IS3_EEPKcRKS3_
	.section	.text._ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_,"axG",@progbits,_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_,comdat
	.weak	_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_
	.type	_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_, @function
_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_:
.LFB2406:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -8[rbp], rdi
	mov	QWORD PTR -16[rbp], rsi
	mov	rax, QWORD PTR -8[rbp]
	movzx	edx, BYTE PTR [rax]
	mov	rax, QWORD PTR -16[rbp]
	movzx	eax, BYTE PTR [rax]
	cmp	dl, al
	sete	al
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2406:
	.size	_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_, .-_ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_
	.section	.text._ZNSt15__new_allocatorIcED2Ev,"axG",@progbits,_ZNSt15__new_allocatorIcED5Ev,comdat
	.align 2
	.weak	_ZNSt15__new_allocatorIcED2Ev
	.type	_ZNSt15__new_allocatorIcED2Ev, @function
_ZNSt15__new_allocatorIcED2Ev:
.LFB2414:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -8[rbp], rdi
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2414:
	.size	_ZNSt15__new_allocatorIcED2Ev, .-_ZNSt15__new_allocatorIcED2Ev
	.weak	_ZNSt15__new_allocatorIcED1Ev
	.set	_ZNSt15__new_allocatorIcED1Ev,_ZNSt15__new_allocatorIcED2Ev
	.section	.text._ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_,"axG",@progbits,_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC5EPS4_,comdat
	.align 2
	.weak	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_
	.type	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_, @function
_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_:
.LFB2444:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -8[rbp], rdi
	mov	QWORD PTR -16[rbp], rsi
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -16[rbp]
	mov	QWORD PTR [rax], rdx
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2444:
	.size	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_, .-_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_
	.weak	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC1EPS4_
	.set	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC1EPS4_,_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC2EPS4_
	.section	.text._ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev,"axG",@progbits,_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD5Ev,comdat
	.align 2
	.weak	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev
	.type	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev, @function
_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev:
.LFB2447:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2447
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], rdi
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR [rax]
	test	rax, rax
	je	.L64
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR [rax]
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@PLT
.L64:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2447:
	.section	.gcc_except_table
.LLSDA2447:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2447-.LLSDACSB2447
.LLSDACSB2447:
.LLSDACSE2447:
	.section	.text._ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev,"axG",@progbits,_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD5Ev,comdat
	.size	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev, .-_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev
	.weak	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD1Ev
	.set	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD1Ev,_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD2Ev
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag,comdat
	.align 2
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag:
.LFB2442:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 96
	mov	QWORD PTR -72[rbp], rdi
	mov	QWORD PTR -80[rbp], rsi
	mov	QWORD PTR -88[rbp], rdx
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	rax, QWORD PTR -80[rbp]
	mov	QWORD PTR -48[rbp], rax
	mov	rax, QWORD PTR -88[rbp]
	mov	QWORD PTR -40[rbp], rax
	nop
	mov	rax, QWORD PTR -48[rbp]
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -40[rbp]
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	sub	rax, QWORD PTR -32[rbp]
	nop
	mov	QWORD PTR -56[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	cmp	rax, 15
	jbe	.L69
	lea	rcx, -56[rbp]
	mov	rax, QWORD PTR -72[rbp]
	mov	edx, 0
	mov	rsi, rcx
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc@PLT
	mov	rdx, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm@PLT
	jmp	.L70
.L69:
	mov	rax, QWORD PTR -72[rbp]
	mov	QWORD PTR -16[rbp], rax
	nop
.L70:
	mov	rdx, QWORD PTR -72[rbp]
	lea	rax, -48[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardC1EPS4_
	mov	rax, QWORD PTR -72[rbp]
	mov	rdi, rax
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEv@PLT
	mov	rcx, rax
	mov	rdx, QWORD PTR -88[rbp]
	mov	rax, QWORD PTR -80[rbp]
	mov	rsi, rax
	mov	rdi, rcx
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_S_copy_charsEPcPKcS7_@PLT
	mov	QWORD PTR -48[rbp], 0
	mov	rdx, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_M_set_lengthEm@PLT
	lea	rax, -48[rbp]
	mov	rdi, rax
	call	_ZZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tagEN6_GuardD1Ev
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L71
	call	__stack_chk_fail@PLT
.L71:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2442:
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag
	.section	.rodata
	.type	_ZNSt8__detail30__integer_to_chars_is_unsignedIjEE, @object
	.size	_ZNSt8__detail30__integer_to_chars_is_unsignedIjEE, 1
_ZNSt8__detail30__integer_to_chars_is_unsignedIjEE:
	.byte	1
	.type	_ZNSt8__detail30__integer_to_chars_is_unsignedImEE, @object
	.size	_ZNSt8__detail30__integer_to_chars_is_unsignedImEE, 1
_ZNSt8__detail30__integer_to_chars_is_unsignedImEE:
	.byte	1
	.type	_ZNSt8__detail30__integer_to_chars_is_unsignedIyEE, @object
	.size	_ZNSt8__detail30__integer_to_chars_is_unsignedIyEE, 1
_ZNSt8__detail30__integer_to_chars_is_unsignedIyEE:
	.byte	1
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
