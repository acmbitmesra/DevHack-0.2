/*function showHTML () {
    document.getElementById('textpad').textContent = editorFrame.document.body.innerHTML;
    return;
    }
*/
var fileCount = localStorage.length;
var oldFileCount = fileCount;
window.addEventListener("load",function(){
    var editor = editorFrame.document;
        editor.designMode = "on";
    
    //show.addEventListener("click",showHTML,false);
    
    boldButton.addEventListener("click",function(){
        editor.execCommand("BOLD", false, null);
    },false);
    
    italicButton.addEventListener("click",function(){
        editor.execCommand("ITALIC", false, null);
    },false);
    
    supButton.addEventListener("click",function(){
        editor.execCommand("Superscript", false, null);
    },false);
    
    subButton.addEventListener("click",function(){
        editor.execCommand("Subscript", false, null);
    },false);

    strikeButton.addEventListener("click",function(){
        editor.execCommand("Strikethrough", false, null);
    },false);

    orderedListButton.addEventListener("click",function(){
        console.log("Ordered Lists : " + orderedListCount);
        editor.execCommand("InsertOrderedList", false, "OL-" + orderedListCount);
        orderedListCount++;
    },false);

    unorderedListButton.addEventListener("click",function(){
        console.log("Unordered Lists : " + unorderedListCount);
        editor.execCommand("InsertUnorderedList", false, "OL-" + unorderedListCount);
        orderedListCount++;
    },false);

    fontColorChange.addEventListener("change",function(event){
        console.log("Font Color Changed to " + event.target.value);
        editor.execCommand("ForeColor", false, event.target.value);
        orderedListCount++;
    },false);

    highlightText.addEventListener("change",function(event){
        console.log("Part of text highlighted with " + event.target.value);
        editor.execCommand("BackColor", false, event.target.value);
        orderedListCount++;
    },false);

    fontChange.addEventListener("change",function(event){
        editor.execCommand("FontName",false, event.target.value);
    },false);

    fontSizeChange.addEventListener("change",function(event){
        editor.execCommand("FontSize",false, event.target.value);
    },false);

    link_url.addEventListener("click",function(){
        var url = prompt("Enter a url","http://");
        editor.execCommand("CreateLink", false, url);
    },false);
    
    undoClick.addEventListener("click",function(){
        editor.execCommand("undo",false,null);
    },false);
    
    redoClick.addEventListener("click",function(){
        editor.execCommand("redo",false,null);
    },false);
    
    saveButton.addEventListener("click",function(){
        fileCount++;
        var fileName = prompt("Save as");
        var fileContent = editorFrame.document.body.innerHTML;
        localStorage.setItem(fileName,fileContent);   
        var option = document.createElement("option");
        option.id = fileName;
        option.text = fileName;
        document.getElementById("openFile").add(option);
        document.getElementById(fileName).selected = "true";
    },false);

    openFile.addEventListener("change",function(event) {
        if(event.target.value == "newFile"){
            editorFrame.document.body.innerHTML = ''; }
        else {
            editorFrame.document.body.innerHTML = localStorage.getItem(event.target.value); }
    },false);

    },false);
