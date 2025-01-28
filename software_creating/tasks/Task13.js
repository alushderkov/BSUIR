/* В строке S могут быть комментарии ( {комме”н”тарий} ) 
то есть заключены в {} , строковые литералы("литерал{л}ы") 
и другие символы. Удалить из строки S все комментарии */

function deleteComments(str) {

  function addToStr(isComment, newStr, elem) {
    
    if (!isComment) {
      newStr += elem;
    }
    
    return newStr;
  }

  let isComment = false;
  let result = ``;

  for (let i = 0; i < str.length; i++) {

    if (str[i] == '{') {
      isComment = true; 
    } else if (str[i] == '}') {
      isComment = false;
    } else {
      result = addToStr( isComment, result, str[i] );
    }
  }

  return result;
}

let myStr = `nve{vdsvbe}vjkbbjkre{wwdvjvsdl}vkdjsb{svjks}"dsvjl"dsvj"kdsvlkn"`;
myStr = deleteComments(myStr);
console.log(myStr);

