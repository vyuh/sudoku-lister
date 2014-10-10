var quote = function(q, c, tags){
	this.q = q
	this.c = c
	this.t = tags
}
var i = new quote("True friends stab you in the front.", "Oscar Wilde", "friends")
console.log(JSON.stringify(i))
