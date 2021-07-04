# Author: xhudak03
# File: main.py
# Project: ISS semestral project (voice processing program due to mask record)

from matplotlib import pyplot as plt
import numpy as np
import soundfile as sf
import IPython
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk
from numpy import pi

def ukol3():
    s, fs = sf.read('maskoffsentence.wav')
    s2, _ = sf.read('maskonsentence.wav')
    print(s.size) # Tisk délek souborů ve vzorcích a v sekundách.
    print(s2.size)
    print(s.size/fs)
    print(s2.size/fs)

    s, fs = sf.read('maskofftone.wav')
    s2, _ = sf.read('maskontone.wav')

    #print(s.size) # Tisk délek souborů ve vzorcích a v sekundách.
    #print(s2.size)
    #print(s.size/fs)
    #print(s2.size/fs)

    t = np.arange(s.size)/fs
    t2 = np.arange(s2.size)/fs

    #_, ploter2 = plt.subplots(2) # Tisk nahrávek. Sloužilo k vyhledávání správných vteřín a jejich rámců
    #ploter2[0].plot(t, s)
    #ploter2[1].plot(t2, s2)

    odkud = 2.3055625
    kolik = 1
    odkud2 = 8.57125

    odkud_vzorky = int(odkud*fs)         # začátek segmentu ve vzorcích
    pokud_vzorky = int((odkud+kolik)*fs) # konec segmentu ve vzorcích

    odkud2_vzorky = int(odkud2*fs)         # začátek segmentu ve vzorcích
    pokud2_vzorky = int((odkud2+kolik)*fs) # konec segmentu ve vzorcích

    s_seg = s[odkud_vzorky:pokud_vzorky]
    s2_seg = s2[odkud2_vzorky:pokud2_vzorky]

    s_seg -= np.mean(s_seg)
    s_seg /= np.abs(s_seg).max()

    s2_seg -= np.mean(s_seg)
    s2_seg /= np.abs(s2_seg).max()

    n = np.arange(s_seg.size)/fs
    n2 = np.arange(s2_seg.size)/fs
    #plt.plot(n, s_seg)
    #plt.plot(n2, s2_seg)
    odkud3 = 0
    kolik2 = 0.02
    odkud3_vzorky = int(odkud3 * fs)         # začátek segmentu ve vzorcích
    pokud3_vzorky = int((odkud3+kolik2) * fs) # konec segmentu ve vzorcích
    s3_seg = s_seg[odkud3_vzorky:pokud3_vzorky]
    s4_seg = s2_seg[odkud3_vzorky:pokud3_vzorky]
    n3 = np.arange(s3_seg.size)/fs
    #_, ploter = plt.subplots()
    #ploter.plot(n3, s3_seg) # printování dobrého rámce
    #ploter.plot(n3, s4_seg) # printování druhého dobrého rámce

    framesoff = np.zeros((99, int(kolik2*fs)))
    frameson = np.zeros((99, int(kolik2 * fs)))
    for i in range(99):
        framesoff[i]=s_seg[int(i*fs*kolik2/2):int(fs*i*kolik2/2+kolik2*fs)]
        frameson[i]=s2_seg[int(i*fs*kolik2/2):int(fs*i*kolik2/2+kolik2*fs)]

    out = framesoff[0]
    outsize = np.arange(out.size)/fs
    out2 = frameson[0]
    outsize2 = np.arange(out2.size)/fs
    #plt.plot(outsize, out)
    #plt.plot(outsize2, out2)
    #plt.show()
    return framesoff, frameson, s_seg, s2_seg

def ukol4(s1_seg, s2_seg):
    max1 = 0
    max2 = 0
    pomseg1 = np.copy(s1_seg)
    pomseg2 = np.copy(s2_seg)
    if np.max(s1_seg)>-np.min(s1_seg):
        max1 = np.max(s1_seg)
    else:
        max1 = -np.min(s1_seg)
    if np.min(s2_seg)>-np.min(s2_seg):
        max2 = np.max(s2_seg)
    else:
        max2 = -np.min(s2_seg)
    for i in range(s1_seg.size):
        if s1_seg[i]>max1*0.7: # zde, pokud se dosadí 0.0 místo 0.7, dojde k vyvolání vícenásobného lagu.
            pomseg1[i]=1
        elif s1_seg[i]<-max1*0.7:
            pomseg1[i]=-1
        else:
            pomseg1[i]=0
    for i in range(s2_seg.size):
        if s2_seg[i]>max2*0.7:
            pomseg2[i]=1
        elif s2_seg[i]<-max2*0.7:
            pomseg2[i]=-1
        else:
            pomseg2[i]=0
    #_, ploter2 = plt.subplots()
    #ploter2.plot(np.arange(s1_seg.size), s1_seg) # tisk rámce bez roušky s prahováním
    #ploter2.plot(np.arange(s2_seg.size), s2_seg) # tisk rámce s rouškou bez prahování
    #plt.show()

    autokorelaceoff = np.zeros(320)
    autokorelaceon = np.zeros(320)
    for i in range(319):
        sumoff = 0
        sumon = 0
        n = 0
        if i<40:
            continue

        while n<(319-i):
            sumoff = sumoff + pomseg1[n]*pomseg1[n+i]
            sumon = sumon + pomseg2[n]*pomseg2[n+i]
            n=n+1
        autokorelaceoff[i] = sumoff
        autokorelaceon[i] = sumon

    where = np.arange(autokorelaceoff.size)
    #plt.plot(where, autokorelaceoff)
    #plt.show()

    frekvenceoff = 16000 / np.argmax(autokorelaceoff)
    frekvenceon = 16000 / np.argmax(autokorelaceon)

    #if frekvenceon>200: # slouží k vykreslení autokorelačních rámců při vyvolání chyby nulovým clippingem
    #    plt.plot(where, autokorelaceon)
    #    plt.show()

    return frekvenceoff, frekvenceon

def ukol5(segment): # ukol
    res = np.zeros((1024,99), np.complex128)
    segment_fftready = []
    #plt.plot(np.hanning(320))
    #plt.plot(10*np.log10(np.abs(np.fft.fft(np.hanning(320), 1024))[:512]**2))
    #plt.show()
    #plt.show()
    for i in range(99):
        windowed_segment = segment[i] # oklika pro případné odkomentování okénkové funkce
        #plt.plot(windowed_segment) # Vykresluje srovnání rámců pro úkol 11
        #windowed_segment = np.hanning(320) * segment[i] # aplikace okénkové funkce
        #plt.plot(windowed_segment)
        #plt.show()
        pom_segment_fftready = np.append(windowed_segment, np.zeros(704))
        segment_fftready = np.append(segment_fftready, pom_segment_fftready)

        #zerosegment = np.append(segment[i], np.zeros(704)) # vlastní implementace DFT - při smazání řádku výše a odkomentování by mělo provádět DFT (pomalé!!!)
        #for k in np.arange(1024):
        #    koef = 0
        #    for n in np.arange(1024):
        #        koef += zerosegment[n]*np.exp(-2j*pi*n*k/1024)
        #    res[k]=koef
    #np.allclose() # funkce pro případné porovnání polí

    segment_fftready.shape=(99, 1024)
    res = np.fft.fft(segment_fftready, 1024)
    returnor = res
    #res = 10 * np.log10(1/1024 * np.abs(res) ** 2)
    #res = np.swapaxes(res, 0, 1)
    #_, graph = plt.subplots()
    #graph.imshow(res[:512]/16000, extent=[0, 1, 0, 8000], origin="lower", aspect="auto")
    #plt.show()
    return returnor

def ukol6(framesoff, frameson, resdftoff, resdfton):
    characteristic = np.zeros((99, 1024), np.complex128)
    framesoffchar = np.copy(framesoff[0])
    framesoffchar = np.append(framesoffchar, np.zeros(704))
    framesonchar = np.copy(frameson[0])
    framesonchar = np.append(framesonchar, np.zeros(704))
    H = np.zeros((99,1024), np.complex128)
    for i in range(99):
        framesoffchar = np.copy(framesoff[i])
        framesoffchar = np.append(framesoffchar, np.zeros(704))
        framesonchar = np.copy(frameson[i])
        framesonchar = np.append(framesonchar, np.zeros(704))
        w, H[i] = freqz(framesonchar, framesoffchar, 1024, fs=16000)
    characteristic = np.zeros((1024,), np.complex128)
    for i in range(99):
        for j in range(1024):
            characteristic[j] += np.abs(H[i][j])
    res = np.abs(characteristic)/99
    #res = 10 * np.log10(np.abs(res) ** 2) # úprava na výkonnostní spektrum - rozbíjí res, takže další výsledky pak nejsou validní
    #plt.plot(w, res) # vykreslení modulu frekvenční charakteristiky roušky
    #plt.show()
    res2pom = np.abs(resdfton)/np.abs(resdftoff) # výpočet frekvenční charakteristiky z mé DFT - velmi velmi podobný tomu z freqz, ale pomalejší... teda, asi spíš ne
    # jenom tomu méně věřím :)
    res2 = np.average(res2pom, axis=0)
    #plt.plot(np.arange(res2.size/2)/512*8000, res2[:512]) # škálování na xové ose zde funguje poněkud naprd, ale co se dá dělat.

    #plt.show()
    return res # pokud se zde vrací místo res res2, pak je frekvenční charakteristika počítaná přes DFT, tedy projeví se pak i okénková funkce.

def ukol7(characteristic):
    idftres = np.fft.ifft(characteristic, 1024) # Rychlá implementace idft

    # Vlastní implementace IDFT - pomalá, funguje po odkomentování
    #xnarray = np.zeros(1024)
    #for n in np.arange(1024):
    #   for k in np.arange(1024):
    #       xnarray[n] += 1/1024 * characteristic[k] * np.exp(+2j*pi*k/1024*n)
    #idftres = xnarray

    plt.plot(idftres[:512]) # vykreslení IDFT (impulzní odezvy)
    plt.show()
    return idftres[:512]

def ukol8(idftres):
    s, fs = sf.read('maskoffsentence.wav')
    s2, fs = sf.read('maskonsentence.wav')
    x = np.arange(s.size)
    _, ahoj = plt.subplots(3)
    ahoj[0].plot(x/fs, s) # Vykreslování nahrávek
    ahoj[1].plot(x/fs, lfilter(x=s, a=idftres, b=[1.0])) #Vykreslení simulace
    ahoj[2].plot(np.arange(s2.size)/fs, s2)
    new = lfilter(x=s, a=idftres, b=[1.0])
    new = np.asarray(new, dtype=np.float32)
    sf.write('sim_maskon_sentence.wav', new, 16000)
    #plt.show()

def ukol8_tone(idftres):
    s, fs = sf.read('maskofftone.wav')
    s2, fs = sf.read('maskontone.wav')
    x = np.arange(s.size)
    _, ahoj = plt.subplots(3)
    ahoj[0].plot(x/fs, s) # Vykreslování nahrávek tónu
    ahoj[1].plot(x/fs, lfilter(x=s, a=idftres, b=[1.0])) # Vykreslení simulace tónu
    ahoj[2].plot(np.arange(s2.size)/fs, s2)
    new = lfilter(x=s, a=idftres, b=[1.0])
    new = np.asarray(new, dtype=np.float32)
    sf.write('sim_maskon_tone.wav', new, 16000)
    plt.show()

def ukol13(framesoff, frameson, framescount, framesoffcomplete, framesoncomplete): # Kopie úkolu 6, akorát upgradovaná
    print(framesoff.shape)
    characteristic = np.zeros((framescount, 1024), np.complex128)
    framesoffchar = np.copy(framesoff[0])
    framesoffchar = np.append(framesoffchar, np.zeros(704))
    framesonchar = np.copy(frameson[0])
    framesonchar = np.append(framesonchar, np.zeros(704))
    H = np.zeros((framescount, 1024), np.complex128)
    for i in range(framescount):
        framesoffchar = np.copy(framesoff[i])
        framesoffchar = np.append(framesoffchar, np.zeros(704))
        framesonchar = np.copy(frameson[i])
        framesonchar = np.append(framesonchar, np.zeros(704))
        w, H[i] = freqz(framesonchar, framesoffchar, 1024, fs=16000)
    characteristic = np.zeros((1024,), np.complex128)
    for i in range(framescount):
        for j in range(1024):
            characteristic[j] += np.abs(H[i][j])
    res = np.abs(characteristic)/framescount
    printres = 10 * np.log10(np.abs(res) ** 2) # úprava na výkonnostní spektrum, takže další výsledky pak nejsou validní
    plt.plot(w, printres) # vykreslení modulu frekvenční charakteristiky roušky
    plt.plot(w, 10 * np.log10(ukol6(framesoffcomplete, framesoncomplete, ukol5(framesoffcomplete), ukol5(framesoncomplete))**2))
    plt.show()
    return res





framesoff, frameson, s_seg1, s_seg2 = ukol3()
#ukol4(framesoff[0], frameson[0]) # sloužilo pouze k vykreslení nultého rámce pro úkol číslo 4
frekvencepoleoff = np.zeros(99)
frekvencepoleon = np.zeros(99)
framesoffdeleted = np.copy(framesoff)
framesondeleted = np.copy(frameson)
framescount = 99
deletor = 0
for i in range(99):
    frekvencepoleoff[i], frekvencepoleon[i]=ukol4(framesoff[i], frameson[i])
    if int(frekvencepoleoff[i]) != int(frekvencepoleon[i]):
        framesoffdeleted = np.delete(framesoffdeleted, deletor, axis=0)
        framesondeleted = np.delete(framesondeleted, deletor, axis=0)
        framescount = framescount - 1
        deletor = deletor - 1
    deletor = deletor + 1

print("Průměr a rozptyl bez masky: ", np.mean(frekvencepoleoff), np.var(frekvencepoleoff))
print("Průměr a rozptyl s maskou: ", np.mean(frekvencepoleon), np.var(frekvencepoleon))
#for i in range(99): Nedokonalé řešení úkolu 11 - nezahrnuje záznam bez roušky
#    if frekvencepoleon[i]>1.5*np.mean(frekvencepoleon):
#        frekvencepoleon[i] = np.median(frekvencepoleon[i-3:i+3]) # bacha na extrémy, tedy že se například extrémní frekvence bude vyskytovat moc vlevo či moc vpravo.
#    if frekvencepoleon[i]<0.5*np.mean(frekvencepoleon):
#        frekvencepoleon[i] = np.median(frekvencepoleon[i-3:i+3])
#plt.plot(np.arange(frekvencepoleoff.size), frekvencepoleoff)
#plt.plot(np.arange(frekvencepoleon.size), frekvencepoleon)
#plt.show()
#ukol5(framesoff) # Provádí fft, docela dlouhé, navíc se provádí pak ještě v parametru dalšího úkolu
#ukol5(frameson)

#pom = ukol6(framesoff, frameson, ukol5(framesoff), ukol5(frameson)) # Vyřešené základní úkoly 5-8
#idftres = ukol7(pom)
#ukol8(idftres)
#ukol8_tone(idftres)

pom = ukol13(framesoffdeleted, framesondeleted, framescount, framesoff, frameson) # řešení úkolu 13, které cca zahrnuje 6-8
idftres = ukol7(pom)
ukol8(idftres)
ukol8_tone(idftres)

