use std::sync::{Arc, Mutex};
use std::thread;
use tungstenite::{connect, Message};
use url::Url;

pub enum WebSocketMessage {
    Text(String),
    Binary(Vec<u8>),
}

pub type WebSocketOnMessage = Box<dyn Fn(WebSocketMessage) + Send + Sync + 'static>;
pub type WebSocketOnOpen = Box<dyn Fn() + Send + Sync + 'static>;
pub type WebSocketOnClose = Box<dyn Fn() + Send + Sync + 'static>;
pub type WebSocketOnError = Box<dyn Fn(String) + Send + Sync + 'static>;

pub struct WebSocketConnection {
    sender: Option<std::sync::mpsc::Sender<Message>>,
    on_message: Arc<Mutex<Option<WebSocketOnMessage>>>,
    on_open: Arc<Mutex<Option<WebSocketOnOpen>>>,
    on_close: Arc<Mutex<Option<WebSocketOnClose>>>,
    on_error: Arc<Mutex<Option<WebSocketOnError>>>,
}

impl WebSocketConnection {
    pub fn new() -> Self {
        Self {
            sender: None,
            on_message: Arc::new(Mutex::new(None)),
            on_open: Arc::new(Mutex::new(None)),
            on_close: Arc::new(Mutex::new(None)),
            on_error: Arc::new(Mutex::new(None)),
        }
    }

    pub fn on_message(&mut self, cb: impl Fn(WebSocketMessage) + Send + Sync + 'static) {
        *self.on_message.lock().unwrap() = Some(Box::new(cb));
    }

    pub fn on_open(&mut self, cb: impl Fn() + Send + Sync + 'static) {
        *self.on_open.lock().unwrap() = Some(Box::new(cb));
    }

    pub fn on_close(&mut self, cb: impl Fn() + Send + Sync + 'static) {
        *self.on_close.lock().unwrap() = Some(Box::new(cb));
    }

    pub fn on_error(&mut self, cb: impl Fn(String) + Send + Sync + 'static) {
        *self.on_error.lock().unwrap() = Some(Box::new(cb));
    }

    pub fn connect(&mut self, url: &str) -> Result<(), String> {
        let (tx, rx) = std::sync::mpsc::channel::<Message>();
        self.sender = Some(tx);

        let url_parsed = Url::parse(url).map_err(|e| e.to_string())?;

        let _on_message = self.on_message.clone();
        let _on_open = self.on_open.clone();
        let _on_close = self.on_close.clone();
        let _on_error = self.on_error.clone();

        thread::spawn(move || {
            let connect_res = connect(url_parsed);
            
            match connect_res {
                Ok((mut socket, _response)) => {
                    // On Open
                    let _cb_open = _on_open.lock().unwrap().take(); // consume or clone? just borrow
                    if let Some(ref _cb) = *_on_open.lock().unwrap() {
                        // how to call safely? We need to run it on main
                        // To avoid lifetime issues, maybe we don't need run_on_main if it's thread-safe, 
                        // but Cocos needs it on main.
                        // Actually, if we use task::run_on_main, we must pass an owned Box<dyn FnOnce>.
                        // But we want to call it multiple times for on_message.
                        // Wait, Fn is implemented for Arc so we can arc-clone the callback.
                    }
                    
                    // Simple event loop
                    loop {
                        // check for sends
                        if let Ok(msg) = rx.try_recv() {
                            if socket.write(msg).is_err() {
                                break;
                            }
                        }
                        
                        // check for recvs (this blocks, so we need a better async loop or timeout)
                        // This is just a stub for now. A real implementation would use non-blocking IO.
                        match socket.read() {
                            Ok(msg) => {
                                match msg {
                                    Message::Text(_t) => {
                                        // invoke on_message
                                    }
                                    Message::Binary(_b) => {
                                        // invoke on_message
                                    }
                                    Message::Close(_) => break,
                                    _ => {}
                                }
                            }
                            Err(_e) => {
                                break;
                            }
                        }
                    }
                    // On Close
                }
                Err(_e) => {
                    // On Error
                }
            }
        });

        Ok(())
    }

    pub fn send(&self, data: &str) -> Result<(), String> {
        if let Some(ref tx) = self.sender {
            tx.send(Message::Text(data.to_string())).map_err(|e| e.to_string())?;
            Ok(())
        } else {
            Err("Not connected".into())
        }
    }

    pub fn send_binary(&self, data: &[u8]) -> Result<(), String> {
        if let Some(ref tx) = self.sender {
            tx.send(Message::Binary(data.to_vec())).map_err(|e| e.to_string())?;
            Ok(())
        } else {
            Err("Not connected".into())
        }
    }

    pub fn close(self) -> Result<(), String> {
        if let Some(ref tx) = self.sender {
            let _ = tx.send(Message::Close(None));
        }
        Ok(())
    }
}

pub struct WebSocketServer {
}

impl WebSocketServer {
    pub fn new(_port: u16) -> Result<Self, String> {
        Ok(Self {})
    }
    pub fn start(&self) -> Result<(), String> {
        Ok(())
    }
    pub fn stop(&self) -> Result<(), String> {
        Ok(())
    }
}
