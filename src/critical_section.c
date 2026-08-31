

/* handwritten syscall stub (EnterCriticalSection): a0=1, syscall */

/**
 * kaze_enter_critical_section  (library, 0x80065cd0)
 *
 * PsyQ EnterCriticalSection wrapper (syscall 0 shim, A36.OBJ).
 */

void kaze_enter_critical_section(void)
{
    __asm__ volatile("addiu	$4, $zero, 0x1\n\tsyscall	0");
}

/* handwritten syscall stub (ExitCriticalSection): a0=2, syscall */

/**
 * kaze_exit_critical_section  (library, 0x80065ce0)
 *
 * PsyQ ExitCriticalSection wrapper (syscall 0 shim, A37.OBJ).
 */

void kaze_exit_critical_section(void)
{
    __asm__ volatile("addiu	$4, $zero, 0x2\n\tsyscall	0");
}
