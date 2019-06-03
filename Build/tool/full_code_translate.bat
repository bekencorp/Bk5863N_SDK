del .\Build\Output\Bk5863N.bin
del .\Build\Output\Bk5863N_crc.bin
C:\Keil\ARM\ARMCC\bin\fromelf.exe  .\Build\Output\Bk5863N.axf   --output .\Build\Output\Bk5863N.bin --bin
.\Build\tool\bk5863n_encrypt.exe .\Build\Output\Bk5863N.bin 00000000 00000000 00000000 00000000
copy .\Build\Output\Bk5863N.bin .\Build\Bk5863N.bin
copy .\Build\Output\Bk5863N_crc.bin .\Build\Bk5863N_crc.bin
copy .\Build\Output\Bk5863N.axf .\Build\Bk5863N.axf