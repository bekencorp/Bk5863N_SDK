del .\Build\Output\user_boot_bk5863N.bin
del .\Build\Output\user_boot_bk5863N_crc.bin
C:\Keil\ARM\ARMCC\bin\fromelf.exe  .\Build\Output\user_boot_bk5863N.axf   --output .\Build\Output\user_boot_bk5863N.bin --bin
.\Build\tool\bk5863n_encrypt.exe .\Build\Output\user_boot_bk5863N.bin 00000000 00000000 00000000 00000000
copy .\Build\Output\user_boot_bk5863N.bin .\Build\user_boot_bk5863N.bin
copy .\Build\Output\user_boot_bk5863N_crc.bin .\Build\user_boot_bk5863N_crc.bin
