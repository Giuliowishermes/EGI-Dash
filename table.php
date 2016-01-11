<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
$db_host = "localhost";
$db_user = "root";
$db_name = "my_innovazioneunito";

// Database connection
$db = mysql_connect($db_host, $db_user);
if ($db == FALSE)
    die("Errore nella connessione.");

$ris = mysql_select_db($db_name);
if ($ris == FALSE)
    die("Errore nella selezione del DB.");





echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
echo "<dati>";

//select massa ultimo pezzo
$sql = "SELECT massa FROM pezzi WHERE id = (SELECT MAX(id) FROM pezzi)";
$ris = mysql_query($sql);
//Se la query va male
if (!$ris) {
    //messaggio di errore per capire il problema
    exit('<p> Errore mentre recuperavo i dati' . mysql_error() . '</p>');
}
// loop per stampare i risultati
while ($riga = mysql_fetch_array($ris)) {
    //struttura del loop visualizzazione si ripeterà n volte
    echo "<massa_ultimo_pezzo>" . $riga['massa'] . "</massa_ultimo_pezzo>";
}//While



//select tempo ultimo pezzo
$sql = "SELECT time FROM pezzi WHERE id = (SELECT MAX(id) FROM pezzi) or id = (SELECT MAX(id) FROM pezzi)-1";
$ris = mysql_query($sql);
//Se la query va male
if (!$ris) {
    //messaggio di errore per capire il problema
    exit('<p> Errore mentre recuperavo i dati' . mysql_error() . '</p>');
}
$tempo;
while ($riga = mysql_fetch_array($ris)) {
    $tempo[] = strtotime($riga['time']);
}
echo "<tempo1pezzo>" . abs($tempo[0] - $tempo[1]) . "</tempo1pezzo>";





//select massa media storico pezzo
$sql = "SELECT avg(massa) as massamedia FROM pezzi";
$ris = mysql_query($sql);
//Se la query va male
if (!$ris) {
    //messaggio di errore per capire il problema
    exit('<p> Errore mentre recuperavo i dati' . mysql_error() . '</p>');
}
$massa_media;
while ($riga = mysql_fetch_array($ris)) {
    $massa_media[] = $riga['massamedia'];
}
echo "<massa_media_dello_storico>" . $massa_media[0]. "</massa_media_dello_storico>";








//select tempo medio dello storico
$sql = "SELECT * FROM pezzi WHERE id = (SELECT MAX(id) FROM pezzi) or id = (SELECT MIN(id) FROM pezzi)";
$ris = mysql_query($sql);
//Se la query va male
if (!$ris) {
    //messaggio di errore per capire il problema
    exit('<p> Errore mentre recuperavo i dati' . mysql_error() . '</p>');
}
$tempo;
$id;
while ($riga = mysql_fetch_array($ris)) {
    $tempo[] = strtotime($riga['time']);
    $id[] = $riga['id'];
}
echo "<tempo_medio_storico_produzione_1_pezzo>" .    abs($tempo[0] - $tempo[1]) /  (abs($id[0] - $id[1]) )  ."</tempo_medio_storico_produzione_1_pezzo>";  //-1 perchè non devo considerare il tempo del primo prodotto non sapendo quando è iniziato








//select temperatura e umidità attuale
$sql = "SELECT * FROM temp_umi WHERE id = (SELECT MAX(id) FROM temp_umi)";
$ris = mysql_query($sql);
//Se la query va male
if (!$ris) {
    //messaggio di errore per capire il problema
    exit('<p> Errore mentre recuperavo i dati' . mysql_error() . '</p>');
}
$temperatura;
$umidita;
while ($riga = mysql_fetch_array($ris)) {
    $temperatura[] = $riga['temp'];
    $umidita[] = $riga['umi'];
}
echo "<temperatura_attuale>" . $temperatura[0] ."</temperatura_attuale>";

echo "<umidita_attuale>" . $umidita[0] ."</umidita_attuale>";






echo "</dati>";
?>

