#include <string>

// Aufgabe 1 [30 Punkte]

// Lösen Sie die Aufgaben, indem Sie Quelltext nach den jeweiligen Kommentaren einfügen.
// Gehen Sie bei den Aufgaben davon aus, dass die Wörter unbekannt sind.
// Wenn Sie z.B. nach "Index von 'hallo' ermitteln" gefragt werden, soll das Wort mit Code gesucht werden und nicht
// direkt der Index fest im Programm festgelegt werden (hardcoding).

int main() {
    /*
    Aufgabe 1.1 [5P]

    Im Ordner "daten" ist eine Datei namens "words.txt".
    Schauen Sie sich zuerst die Datei an.
    Die erste Zeile ist der Header. Überspringen Sie diese Zeile und lesen Sie dann alle Wörter aus dieser Datei in
    einen geeigneten std::vector ein (ein Wort = ein Element im Vektor).

    Achtung: Wenn Sie die Schaltflächen unten in der blauen Leiste (anstatt F5) verwenden, stimmt das Arbeitsverzeichnis
    nicht. Falls Sie die Schaltflächen unbedingt verwenden möchten, muss vor dem Dateipfad der zu öffnenden Datei "../"
    stehen.

    Wenn ratlos: Denken Sie sich einen Vektor mit 10 Wörtern aus und fahren Sie fort.
    */



    /*
    Aufgabe 1.2 [2P]

    Geben Sie das erste Wort im Vektor aus, dass mit einem Kleinbuchstaben endet.
    
    Es existiert mindestens ein Wort, dass diese Bedingung erfüllt.
    
    Die Funktionen im Header <cctype> können hier hilfreich sein.
    */



    /*
    Aufgabe 1.3 [2P]

    Geben Sie nachfolgende Zeile auf der Konsole aus (ohne ""):
    "Das fuenfte Wort ist: [WORT]"
    
    Ersetzen Sie [WORT] mit dem fünften Wort aus dem Vektor.
    */



    /*
    Aufgabe 1.4 [3P]

    Fragen Sie den Benutzer nach seinem Studiengang und fügen Sie diesen dann als viertes
    Element in den Vektor ein.
    */



    /*
    Aufgabe 1.5 [1P]

    Fügen Sie Ihre eigene Matrikelnummer als letztes Element in den Vektor ein.
    */



    /*
    Aufgabe 1.6 [5P]

    Erstellen Sie in "aufgabe1" die Dateien utility.h und utility.cpp.
    Implementieren Sie in diesen Dateien eine Funktion namens "replace".
    
    An diese Funktion soll ein Vektor übergeben werden können, der von dieser Funktion direkt modifiziert wird.
    In dem übergebenen Vektor soll in jedem Wort das Zeichen an der 5. Stelle (sofern vorhanden) mit 'x' ersetzt werden.
    Die Funktion hat keinen Rückgabewert (void).
    
    Übergeben Sie Ihren Vektor an diese Funktion.
    */



    /*
    Aufgabe 1.7 [3P]

    Erstellen Sie in der gleichen Datei wie in Aufgabe 1.6 eine weitere Funktion namens "counter".
    
    An diese Funktion soll ein Vektor übergeben werden können. Die Funktion garantiert, dass der Vektor nicht
    kopiert und nicht modifiziert wird.
    
    Die Funktion zählt, wie viele Wörter im Vektor weniger als 6 Zeichen haben und gibt dann deren Anzahl zurück.
    
    Rufen Sie diese Funktion ebenfalls auf.
    */



    /*
    Aufgabe 1.8 [3P]

    Löschen Sie aus dem Vektor alle Wörter, die ein 'a' enthalten.
    
    Hinweis: Hierfür können Sie die Methode 'find' der Klasse std::string verwenden.
    */



    /*
    Aufgabe 1.9 [2P]

    Berechnen Sie für die verbleibenden Wörter im Vektor:
    - Die Anzahl an Zeichen über alle Wörter (Summe)
    - Die durchschnittliche Zeichenanzahl pro Wort
    */



    /*
    Aufgabe 1.10 [5P]

    Schreiben Sie den Vektor in eine Datei. Den Dateinamen können Sie beliebig wählen.
    Das Dateiformat soll folgendermaßen aussehen:
    1. Zeile: Alle Wörter des Vektors, getrennt durch Doppelpunkt (:).
    2. Zeile: Die Summe aus Aufgabe 1.9
    3. Zeile: Der Durchschnitt aus Aufgabe 1.9
    
    Beispielausgabe:
    Hallo:welt:1234:ein_wort
    21
    5.25
    */



    return 0;
}
