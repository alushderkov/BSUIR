/* Найти процентное отношение в строке символа, введенного с клавиатуры */

function calcDif(symb, arr) {
  let result = arr.length;

  for (let i = 0; i < arr.length; i++) {

    if (arr[i] == symb) {
      arr.splice(i, 1);
      i--;
    }
  }

  console.log(arr);
  return ( (result - arr.length) *  100 / result ) + '%';
}

let myArr = ['a', 'f', 'f', 't', 'n'];

console.log( calcDif('f', myArr) );