var express= require("express");
var app =express();
var bodyParser  =require("body-parser");
var mongoose    =require("mongoose");

mongoose.connect("mongodb://localhost:27017/editor", { useNewUrlParser: true });

app.set("view engine","ejs");

app.use(express.static("public"));
app.use(bodyParser.urlencoded({extended:true}));


var docSchema=new mongoose.Schema({
    name:String,
    author:String,
    content:String
});

//model fromation
var Doc= mongoose.model("Doc",docSchema);


app.get("/",function(req, res){
    res.render("index");
});


app.get('/docs',function(req,res){
    Doc.find(function(err ,docs){
        if(err)
            console.log(err);
        else 
        res.render("doc_list",{docs:docs});
    });
});


var num=1;
app.post("/docs/save",function(req, res){
    var new_doc=req.body;
    if(new_doc['name']=='')
        new_doc.name="doc"+(num++);
    Doc.create(new_doc, function(err,newly_created){
        if(err)
        console.log(err);
        else{
        var red_url="/docs/"+newly_created.id;
        res.send(red_url);
    }
    });
});
app.post("/docs/update",function(req, res){
    var new_doc=req.body;
    var update_data={
        name:req.body.name,
    author:req.body.author,
    content:req.body.content
    }
    Doc.findOneAndUpdate({id:new_doc.id},update_data,function(err,updated){
        if(err)
        console.log(err);
        else{
        res.send("success");
    }
    });
});
app.get("/docs/:id",function(req,res){
    
    Doc.findById(req.params.id,function(err,matched_doc){
        if(err){
            res.send("Document not found");
        }
        else{
            res.render("preview",{doc:matched_doc});
        }
    });
});

app.listen(process.env.PORT , process.env.IP ,function(){
    console.log(" Server Started!!....");
});