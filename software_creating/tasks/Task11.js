/* Перевод в п-ую сс из десятичной */

function outputArr(arr) {
  let result = ``;

  for (let i = 0; i < arr.length; i++)
    result += arr[i];

  return result;
}

function ssTranslate (Numb, ss) {
  let result = [] 
  let int, dif, dist;
  
  while (Numb >= 1) {
    int = Numb % ss;
    dif = 'A'.charCodeAt(0) - '9'.charCodeAt(0) - 1;

    if (int > 9) {
      dist = int - 9;
      int = String.fromCharCode( ('9'.charCodeAt(0) + dist + dif) );
    }

    result.unshift(int);
    Numb = Math.floor(Numb / ss); 
  }

  return result;
}

let myNumb = ssTranslate(247, 16);
console.log( outputArr(myNumb) );