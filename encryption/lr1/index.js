"use strict"

// Два алгоритма - предоставить выбор
// Несколько файлов с тестами
// Файл с выводом результата
// Отчёт с тестами и визуализацией
// Для алгоритма Виженера в отчёте необходимо изобразить таблицу
// Отдельно два ключа: чётный, нечётный для "ж/д изгороди"
// Для русского языка буква "ё" также должна обрабатываться
// Ключ вводится с клавиатуры и никуда не записывается
// Если на вход шифратора подан уже зашифрованный текст, это не значит,
// что после шифрования он снова преобразуется в исходный текст:
// ЭТО СЛУЧАЙНОСТЬ И САМЫЙ НЕЖЕЛАЕМЫЙ СЛУЧАЙ

/* Методы алгоритма к реализации:

  * метод адаптации ключа:
    - на вход метода: ключ
    - задача: выделить рабочий ключ
    - на выходе: рабочий ключ или индикатор неверного ключа (если ни один
    элемент не подходит)

  * метод шифрования: только заданный текст, остальные символы игнорировать
    - на вход метода:
      * исходный текст
      * ключ
    - особенности:
      * в алгоритме "ж/д изгороди" сделать проверку на чётность/нечётность ключа
        отдельной функцией и, вероятно, тернарным оператором выполнить две
        различные функции под каждый ключ
      * для функций под разные ключи написать "подфункции", если код будет
        похож, чтобы он не совпадал

  * метод дешифрирования:
    - на вход метода:
      * зашифрованный текст
      * ключ
    - на выходе: исходный текст

  ЗАДАЧА: * наследовать от класса "алгоритм" два подкласса для разных алгоритмов
            и переопределить методы на специализированные

          * общие методы реализовать в базовом классе "алгоритм"

 */

class Algorithm {

  readFile(input) {
    let file = input.files[0];

    let reader = new FileReader();

    try {
      reader.readAsText(file);
      return reader.result;

    } catch (err) {

      //alert(`При чтении файла возникла ошибка ${reader.error}`);
      return ``;
    }
  }

  writeFile(link, text) {
    let data = new Blob([text],
      {type: 'text/plain'});
    let url = window.URL.createObjectURL(data);
    let fileName = `output.txt`;

    link.href = url;
    link.download = fileName;
  }

  formatText(string) {
    const regExp = /[а-яё]/;
    let result = ``;
    string = string.toLowerCase();

    for (let i = 0; i < string.length; i++) {

      if ( regExp.test( string[i] ) ) {
        result += string[i];
      }
    }

    return result;
  }

  keyAdaptation(key) {};
  ciphering(plain_text, key) {};
  decoding(cipher_text, key) {};


}

class Rail_Fence extends Algorithm {

  keyAdaptation(key) {

    if (key) {
      const regExp = /[0-9]/;
      let result = ``;

      for (let i = 0; i < key.length; i++) {

        if (regExp.test(key[i])) {
          result += key[i];
        }
      }

      return result;

    } else {

      return ``;
    }
  }

  isValidKey(key) {
    let result = true;

    if (key === ``) {
      //alert(`Ошибка! Ключ не распознан, попробуйте ввести заново`);
      result = false;
    }

    return result;
  }

  isNecessaryToCipher(plain_text, key) {
    let result = true;

    if (key === `0` || key === `1` || key.length >= plain_text.length) {
      result = false;
    }

    return result;
  }

  ciphering(plain_text, key) {
    let cipher_text = ``;
    function cipher() {
      const result = new Array(key).fill("");

      let direction = 1;
      let currentRail = 0;

      for (let char of plain_text) {
        result[currentRail] += char;
        currentRail += direction;

        if (currentRail === 0 || currentRail === key - 1) {
          direction *= -1;
        }
      }

      return result.join("");
    }

    if ( !this.isValidKey(key) ) {
      return cipher_text;
    }

    if( !this.isNecessaryToCipher(plain_text, key) ) {
      cipher_text = plain_text;
      return cipher_text;
    }

    cipher_text = cipher();

    return cipher_text;

  }

  decoding(cipher_text, key) {
    let plain_text = ``;

    function decode() {
      let result = ``;
      const railPositions = new Array(cipher_text.length).fill(0);

      let direction = 1;
      let currentRail = 0;

      for (let i = 0; i < cipher_text.length; i++) {
        railPositions[i] = currentRail;
        currentRail += direction;

        if (currentRail === 0 || currentRail === key - 1) {
          direction *= -1;
        }
      }

      const rails = new Array(key).fill("").map(() => []);

      let index = 0;
      for (let i = 0; i < key; i++) {
        for (let j = 0; j < cipher_text.length; j++) {
          if (railPositions[j] === i) {
            rails[i].push(cipher_text[index]);
            index++;
          }
        }
      }

      currentRail = 0;
      direction = 1;

      for (let i = 0; i < cipher_text.length; i++) {
        result += rails[currentRail].shift();
        currentRail += direction;

        if (currentRail === 0 || currentRail === key - 1) {
          direction *= -1;
        }
      }

      return result;

    }

    if ( !this.isValidKey(key) ) {
      return plain_text;
    }

    if( !this.isNecessaryToCipher(cipher_text, key) ) {
      plain_text = cipher_text;
      return plain_text;
    }

    plain_text = decode();

    return plain_text;
  }



}

class Vigenere extends Algorithm {
  LetterCount = 33;
  keyAdaptation(key) {
    return this.formatText(key);
  }

  shiftKey(key) {
    for (let i = 0; i < key.length; i++) {
      if (key[i] === 'я') {
        key[i] = 'а';
      } else if (key[i] === 'ё') {
        key[i] = 'ж';
      } else {
        key[i] = String.fromCharCode(key[i].charCodeAt(0) + 1);
      }
    }
    return key;
  }

  ciphering(plainText, key) {

    if (key === "") return "";

    let resultText = plainText.toLowerCase();
    let progressiveKey = key.split('');
    let letterArray = new Array(this.LetterCount);
    let letter = 0;

    for (let i = 'а'.charCodeAt(0); i <= 'я'.charCodeAt(0); i++) {
      if (letter === 6) {
        letterArray[letter++] = 'ё';
      }
      letterArray[letter++] = String.fromCharCode(i);
    }

    let sbCipherText = '';
    let index = 0;
    let c = 0;

    for (let i = 0; i < resultText.length; i++) {
      let keyLetter = key[index % key.length];
      let changedLetter;

      if (resultText[i] === 'ё') {
        changedLetter = 6;
      } else {
        changedLetter = resultText[i] <= 'е' ? resultText[i].charCodeAt(0) - 'а'.charCodeAt(0) : resultText[i].charCodeAt(0) - 'а'.charCodeAt(0) + 1;
      }

      switch (keyLetter) {
        case 'ё':
          sbCipherText += letterArray[(changedLetter + 6) % this.LetterCount];
          break;

        case 'а':
        case 'б':
        case 'в':
        case 'г':
        case 'д':
        case 'е':
        {
          let changedKeyLetter = keyLetter.charCodeAt(0) - 'а'.charCodeAt(0);
          sbCipherText += letterArray[(changedLetter + changedKeyLetter) % this.LetterCount];
          break;
        }

        default:
        {
          let changedKeyLetter = keyLetter.charCodeAt(0) - 'а'.charCodeAt(0) + 1;
          sbCipherText += letterArray[(changedLetter + changedKeyLetter) % this.LetterCount];
          break;
        }
      }

      index++;
      c++;
      if (c === key.length) {
        progressiveKey = this.shiftKey(progressiveKey);
        key = progressiveKey.join('');
        c = 0;
      }
    }

    return sbCipherText;
  }

  decoding(cipher, key) {
    if (key === "") return "";

    let resultText = cipher.toLowerCase();
    let progressiveKey = key.split('');
    let letterArray = new Array(this.LetterCount);
    let letter = 0;

    for (let i = 'а'.charCodeAt(0); i <= 'я'.charCodeAt(0); i++) {
      if (letter === 6) {
        letterArray[letter++] = 'ё';
      }
      letterArray[letter++] = String.fromCharCode(i);
    }

    let sbPlainText = '';
    let index = 0;
    let c = 0;

    for (let i = 0; i < resultText.length; i++) {
      let keyLetter = key[index % key.length];
      let changedLetter;

      if (resultText[i] === 'ё') {
        changedLetter = 6;
      } else {
        changedLetter = resultText[i] <= 'е' ? resultText[i].charCodeAt(0) - 'а'.charCodeAt(0) : resultText[i].charCodeAt(0) - 'а'.charCodeAt(0) + 1;
      }

      switch (keyLetter) {
        case 'ё':
          sbPlainText += letterArray[(changedLetter + (this.LetterCount - 6)) % this.LetterCount];
          break;

        case 'а':
        case 'б':
        case 'в':
        case 'г':
        case 'д':
        case 'е':
        {
          let changedKeyLetter = keyLetter.charCodeAt(0) - 'а'.charCodeAt(0);
          sbPlainText += letterArray[(changedLetter + (this.LetterCount - changedKeyLetter)) % this.LetterCount];
          break;
        }

        default:
        {
          let changedKeyLetter = keyLetter.charCodeAt(0) - 'а'.charCodeAt(0) + 1;
          sbPlainText += letterArray[(changedLetter + (this.LetterCount - changedKeyLetter)) % this.LetterCount];
          break;
        }
      }

      index++;
      c++;
      if (c === key.length) {
        progressiveKey = this.shiftKey(progressiveKey);
        key = progressiveKey.join('');
        c = 0;
      }
    }

    return sbPlainText;
  }


}

const alg = new Vigenere();
console.log( alg.ciphering(`криптография`,`абве`) );
console.log( alg.decoding(`кскфурёчвчмж`,`абве`) );