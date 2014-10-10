var quote = function(q, c, tags){
	this.q = q
	this.c = c
	this.t = tags
}
quote.prototype.author = function() {
	console.log(this.c)
}
var i = new quote("True friends stab you in the front.", "Oscar Wilde", "friends")
console.log(JSON.stringify(i))
i.author()