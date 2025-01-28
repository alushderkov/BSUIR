/* Вводится два числа М и N и массив размерностью M*N, найти индекс 
столбца в котором сумма элементов максимальна */

function findMaxColumn(arr) {
  let arrSize = arr.length;
  let rowSize = arr[0].length;
  let result = [];

  function sum(column) {
    let result = 0;
  
    for (let i = 0; i < arrSize; i++) {
      result += arr[i][column];
    }
  
    return result;
  }

  for (let i = 0; i < rowSize; i++) {
    result.push( sum(i) );
  }

  return result.indexOf( Math.max(...result) );
}

let myArr = [
  [3, 5, 9],
  [2, 7, 8],
  [1, 7, 3],
  [2, 0, 5]
]

console.log( findMaxColumn(myArr) );