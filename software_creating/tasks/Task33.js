/* Есть массив байтов вводить как хочешь. 
Надо проверить закодирована ли она с помощью UTF-8. 
Размер символа от 1 до 4 байт. и там табличка с масками. 

1 байт: 0xxxxxxx
2 байта: 110xxxxx 10xxxxxx
3 байта: 1110xxxx 10xxxxxx 10xxxxxx
4 байта: 11110xxx  xxxxxx 10xxxxxx 10xxxxxx

Где x произвольное */

function isUTF8(arr) {
  let bitCount = Math.ceil(arr.length / 8);

  switch (bitCount) {

    case 1: {

      if (!arr[0]) {
        return true;
      }
    } 
    break;

    case 2: {

    } 
    break;

    case 3: {

    } 
    break;

    case 4: {

    } 
    break;
  }

}