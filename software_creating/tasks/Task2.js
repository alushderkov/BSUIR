/*Есть массив 9 8 7 6 5 4 3 2 1
Сделать 9 7 5 3 1 2 4 6 8	
Нечетные записать сначала в том же порядке, 
в котором они встречаются, четные - наоборот ( записать с конца)*/

function changeArray (arr) {
  let result;
  let evenArr = [];
  let tempArr = [];

  for (let elem of arr) {

    if (elem % 2 == 0) {
      evenArr.unshift(elem);
    } else {
      tempArr.push(elem);
    }
  }

  result = tempArr + ',' + evenArr;

  return result
}

let arr = [9, 8, 7, 6, 5, 4, 3, 2, 1];
arr =  changeArray(arr);
console.log(arr);
