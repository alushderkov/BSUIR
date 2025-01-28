/* Дана строка, состоящая из идентификаторов и чисел, 
разделенных пробелами. Необходимо сформировать строку, 
состоящую из чисел, разделенных запятыми. (Проверки не надо) */

function createNumbStr(str) {
  let result = ``;
  let arr = str.split(' ');

  for (let elem of arr) {

    if ( isFinite( elem[0] ) ) {
      result += elem + ', ';
    }
  }
  
  return result;
}

let myStr = `1235 ssjk45 gkj3 234 hbv6h54 4567 1 0 fwfwf`;

myStr = createNumbStr(myStr);

console.log( myStr.slice(0, myStr.length - 2) );