/* НОД методом Эвклида */

function calcNOD(a, b) {
  let big = Math.max(a, b);
  let small = Math.min(a, b);
  let mod = 1;

  while (mod) {
    mod = big % small;

    if (mod) {
      big = small;
      small = mod;
    } else {
      return small; 
    }
  }
}

console.log( calcNOD(256, 48) );