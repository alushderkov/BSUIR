/* За один проход найти 2 минимальных числа */

function findTwoMin(arr) {
  let result = [ arr[0], arr[1] ];

  for (let i = 2; i < arr.length; i++) {

    if ( arr[i] < result[2] && arr[i] > result[1] ) {
      result[2] = arr[i];
    } else if ( arr[i] < result[1] ) {
      result [1] = arr[i];
    }
  }

  return result;
}

let myArr = [2, 4, 3, 7, 8, 9, 13, 5, 6, -14, 9, 14];

console.log( findTwoMin(myArr) );