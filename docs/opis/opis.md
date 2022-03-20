# Przetwarzanie wideo przy uzyciu shaderow

## Cel programu

Program pozwala na modyfikacje klatek pliku wideo
przez program/y napisane w jezyku GLSL.

## Opis programu

Program przyjmuje na wejscie sciezke do skryptu, w ktorym mozna zdefiniowac

- plik/i wejsciowe
- plik/i wyjsciowe
- shadery
- instrukcje do wykonania

Nastepnie laduje i przygotowuje wyzej wymienione zasoby.

Pliki wejsciowe opakowywane sa jako zrodlo danych,
ktore pozwala na pozyskanie nastepnej klatki o okreslonym formacie.
Implementacja wykorzystuje biblioteke ffmpeg do demuxowania i dekodowania wideo

Analogicze wyglada sytuacja plikow wyjsciowych.

Tworzony jest kontext OpenGL a nastepnie ladowane i kompilowane wszystkie programy GLSL

Nastepnie interpreter wykonuje sekcje zawierajaca operacje na plikach wejscia/plikach wyjscia i shaderach.

Wyniki tych operacji, jezeli tak stanowi skrypt
moga byc wypisane do pliku wyjscia jako zmodyfikowane klatki.

