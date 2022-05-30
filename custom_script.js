//вызывает функцию мульт с помощью параметров формы и загружает результаты в форму после ответа функции
document.querySelector('#btn-start-calc').addEventListener('click', function startCalculation() {
    const inputs = getFormInputs();
    let resp;
     resp = JSON.parse(mult(
        inputs.a.value,//a
        inputs.b.value,//b
        inputs.p.value, //p
        inputs.x.value,//x
        inputs.y.value,//y
        inputs.k.value //k
        ));

    inputs.qx.value = resp.x;
    inputs.qy.value = resp.y;
    inputs.time.value = resp.time;
});
//вставка сохраненных значений
document.querySelector('#btn-paste-from-stb').addEventListener('click', function pasteValuesFromStb() {
    const inputs = getFormInputs();

        inputs.a.value = 'FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC';
	inputs.b.value = '5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b',//b
        inputs.p.value = 'FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF', //p
        inputs.x.value = 'FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC',//x
        inputs.y.value = 'FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC',//y
        inputs.k.value = '70B99F5ADCFDBEF81058967018BA95736AA2B60060EC25FFFFFFFFFFFFFFFFFF' //k
});
//функция вытягивается из wasm модуля
function mult (a, b, p, pointX, pointY, multK) {
    return Module.ccall(
        'mult',	// name of C function
        'string',	// return type
        ['string', 'string', 'string', 'string', 'string', 'string'],	// argument types
        [a, b, p, pointX, pointY, multK]	// arguments
    );
}

//эта функция нужна чтобы обращаться к полям формы и их знаечниям, изменять их и вытягивать.
function getFormInputs(values = false) {
    const form = document.querySelector('#form');
	
    if (values) {
        return {
            a: form.elements['curve-a'].value,
	b: form.elements['curve-b'].value,
	p: form.elements['curve-p'].value,
            time: form.elements['curve-time'].value,
            x: form.elements['curve-x'].value,
            y: form.elements['curve-y'].value,
	k: form.elements['curve-k'].value,
	qx: form.elements['curve-qx'].value,
            qy: form.elements['curve-qy'].value
        }
    } else {
        return {
              a: form.elements['curve-a'],
	b: form.elements['curve-b'],
	p: form.elements['curve-p'],
            time: form.elements['curve-time'],
            x: form.elements['curve-x'],
            y: form.elements['curve-y'],
	k: form.elements['curve-k'],
	qx: form.elements['curve-qx'],
            qy: form.elements['curve-qy']
        }
    }
}