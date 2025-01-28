/* Дано число N. Вывести степени двойки от первой до N-ой */

function printTwo(N) {
  let result = 1;

  for (let i = 1; i <= N; i++) {
    result *= 2;
    console.log(result);
  }
}

printTwo(25);