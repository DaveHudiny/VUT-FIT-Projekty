Dobrý den.

archiv obsahuje kódy mobilní aplikace pro platformu Android. V případě, že spouštějící nechce vytvářet aplikaci sám, může stáhnout řešení
ve formě .apk souboru na odkazu: https://drive.google.com/drive/u/0/folders/12lj04a_bYxYjQR7yN0sB6TxlIn0AWdyh

pro samotné zprovoznění projektu je potřeba Visual Studio (testováno pouze na verzi 2019). Do něj je třeba stáhnout rozšíření pro zprovoznění
.Netu a především rozšíření pro vývoj mobilních aplikací (Xamarin). Při buildění by se pak mělo vytvořit Android Studio, kde aplikace byla
testována na defaultním nabízenem telefonu (Pixel). VAROVÁNÍ - překlad může trvat opravdu hodně dlouho, mně první spuštění zabralo i 15 minut. 
Pokud nebude aplikace zkompilovatelná, zřejmě bude potřeba stáhnout NuGet rozšíření(při mém zkoušení na jiném zařízení potřeba nebyly). Jedná se konkrétně o: XamForms.Controls.Calendar, 
Xamarin.Forms, Xamarin.Essentials. Pokud by daná rozšíření nestačila, tak v projektu máme stažené i Google.Apis.Calendar.v3, NETStandard.Library 
a System.Globalization.Calendars (tato rozšíření nebyla použita, ale nejsem si jist, jestli je projekt nebude nějakým způsobem požadovat.) Posledním problémem by mohl
být Newtonsoft.Json. Ten se neukázal jako nutný pro spuštění, ale je poslední variantou, co může chybět. Pokud by nadále byl problém se
zprovozněním, kontaktujte prosím - xhudak03@vutbr.cz

Děkuji za přečtení tohoto readme.txt
Přeji hezké svátky.