/* Ввод чисел N и K, K < N. Создать массив из N элементов 
(элементы начинаются с 1-го) удалить из массива К-й элемент. 
Вывести преобразованный массив, отводя под каждый элемент 7 
экранных позиций */

function random(min, max) {
  let result = min + Math.random() * (max - min);
  return Math.floor(result);
}

function createArray(N) {
  let result = new Array(N);

  for (let i = 0; i < N; i++) {
    result[i] = random(0, 10);
  }

  return result;
}

function delElemArray(K, arr) {
  let N = arr.length;
  let result = [];

  if (K < N) {
    return result.concat( arr.slice(0, K), arr.slice(K + 1) );
  } else {
    console.log('Число K больше размера массива N');
    return arr;
  }
}

let N = 10;
let K = 11;
let myArray = createArray(N);

console.log(myArray);

myArray = delElemArray(K, myArray);

console.log(myArray);