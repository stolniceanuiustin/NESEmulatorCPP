Emulator NES – Proiect personal în C++
Am dezvoltat un emulator funcțional pentru consola Nintendo Entertainment System (NES), care permite rularea jocurilor originale (ROM-uri) pe orice platformă compatibilă cu un compilator C++.
Componente emulate:
CPU – Ricoh 2A03:
 Am implementat un nucleu compatibil cu procesorul 6502 modificat, fără suport pentru instrucțiunile BCD. Emularea include mecanismul de fetch-decode-execute, tratarea întreruperilor (IRQ/NMI) și sincronizarea precisă a ciclurilor de ceas conform specificațiilor originale.
PPU – Ricoh 2C02:
 Am reprodus unitatea grafică responsabilă de afișarea imaginii. Funcționalitatea include randarea pe scanline, paletele de culori, afișarea sprite-urilor și logica pentru registrele grafice.
Controller NES:
 Am emulat comportamentul porturilor de intrare bazate pe shift register, permițând maparea tastaturii moderne la butoanele clasice NES (A, B, Start, Select, direcționale).
Mapare memorie – Mapper 0 (NROM):
 Suport pentru cartridge-uri standard, fără bank switching. Plănuiesc extinderea suportului pentru mappere mai complexe (ex: MMC1, MMC3) pentru a rula o gamă mai largă de jocuri. In prezent functioneaza bine Donkey Kong si Super Mario Bros.
Tehnologii utilizate:
C++ 
SDL2 – pentru afișare grafică și gestionarea input-ului
Meson - Build System
Rezultate și beneficii:
Acest proiect mi-a oferit o înțelegere practică și detaliată asupra:
Arhitecturii procesoarelor și interpretării codului mașină
Sincronizării și comunicării între componente hardware simulate
Principiilor de funcționare ale graficii 2D bazate pe rasterizare
Proiectul a fost motivat de pasiunea mea pentru emulatoare, dezvoltându-se din dorința de a înțelege în profunzime cum funcționează consolele clasice și de a-mi construi propriul emulator de la zero.
