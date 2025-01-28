/*Найти в строке, содержащей слова и числа, разделенные пробелами, количество
вхождений числа М*/

function countNumber(M, str) {
  let result = 0;

  if ( isFinite(M) ) {
    let pos = -1;

    while (pos) {
      pos = str.indexOf(M, pos) + 1;

      if (pos) {
        result++;
      }
    }
  } else {
    result = 'M не является числом';
  }
  
  return result;
}

const M = 123;
const myString = ``;

console.log( `Количество вхождений числа M: ${ countNumber(M, myString) }` );