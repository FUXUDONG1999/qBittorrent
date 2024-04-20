'use strict';

document.addEventListener('DOMContentLoaded', function () {
    document.getElementById('username').focus();
    document.getElementById('username').select();
});

function submitLoginForm(event) {
    event.preventDefault();
    const errorMsgElement = document.getElementById('error_msg');

    const xhr = new XMLHttpRequest();
    xhr.open('POST', 'api/v2/auth/login', true);
    xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded; charset=UTF-8');
    xhr.addEventListener('readystatechange', function () {
        if (xhr.readyState === 4) { // DONE state
            if ((xhr.status === 200) && (xhr.responseText === "Ok."))
                location.reload(true);
            else
                errorMsgElement.textContent = 'QBT_TR(Invalid Username or Password.)QBT_TR[CONTEXT=HttpServer]';
        }
    });
    xhr.addEventListener('error', function () {
        errorMsgElement.textContent = (xhr.responseText !== "")
            ? xhr.responseText
            : 'QBT_TR(Unable to log in, qBittorrent is probably unreachable.)QBT_TR[CONTEXT=HttpServer]';
    });

    const usernameElement = document.getElementById('username');
    const passwordElement = document.getElementById('password');
    const queryString = "username=" + encodeURIComponent(usernameElement.value) + "&password=" + encodeURIComponent(passwordElement.value);
    xhr.send(queryString);

    // clear the field
    passwordElement.value = '';
}
