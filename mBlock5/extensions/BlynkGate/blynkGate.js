// class Queue {
//     constructor() {
//         this.items = [];
//     }

//     // Thêm một phần tử vào hàng đợi
//     enqueue(key, value) {
//         this.items.push({ key: key, value: value });
//     }

//     // Lấy một phần tử ra khỏi hàng đợi
//     dequeue() {
//         if (this.isEmpty()) {
//             return "Queue is empty";
//         }
//         return this.items.shift();
//     }

//     // Kiểm tra hàng đợi có rỗng hay không
//     isEmpty() {
//         return this.items.length === 0;
//     }

//     // Lấy phần tử đầu tiên của hàng đợi mà không xóa nó
//     front() {
//         if (this.isEmpty()) {
//             return "Queue is empty";
//         }
//         return this.items[0];
//     }

//     // Lấy kích thước của hàng đợi
//     size() {
//         return this.items.length;
//     }

//     // In ra tất cả các phần tử trong hàng đợi
//     printQueue() {
//         let str = "";
//         for (let i = 0; i < this.items.length; i++) {
//             str += `{ key: ${this.items[i].key}, value: ${this.items[i].value} } `;
//         }
//         return str;
//     }
// }

// // Ví dụ sử dụng
// let queue = new Queue();
// queue.enqueue("v1", 10);
// queue.enqueue("v2", 20);
// queue.enqueue("v3", 30);

// let queueTX = new Queue();
// queueTX.enqueue("v1", 100);
// queueTX.enqueue("v2", 200);
// queueTX.enqueue("v3", 300);

// console.log("queue.printQueue()",queueTX.printQueue()); // In ra hàng đợi
// console.log("queue.dequeue()",queueTX.dequeue()); // Lấy phần tử đầu tiên ra khỏi hàng đợi
// console.log("queue.printQueue()",queueTX.printQueue()); // In ra hàng đợi sau khi lấy phần tử đầu tiên
// console.log("queue.front()", queueTX.front()); // Lấy phần tử đầu tiên mà không xóa nó
// console.log("queue.size()", queueTX.size()); // Lấy kích thước của hàng đợi


// console.log("queue.printQueue()",queue.printQueue()); // In ra hàng đợi
// console.log("queue.dequeue()",queue.dequeue()); // Lấy phần tử đầu tiên ra khỏi hàng đợi
// console.log("queue.printQueue()",queue.printQueue()); // In ra hàng đợi sau khi lấy phần tử đầu tiên
// console.log("queue.front()", queue.front()); // Lấy phần tử đầu tiên mà không xóa nó
// console.log("queue.size()", queue.size()); // Lấy kích thước của hàng đợi

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Lấy phần tử đầu tiên của hàng đợi mà không xóa nó (peek)
// Chuyển đổi từ chuỗi JSON vào hàng đợi
// Chuyển đổi các phần tử của hàng đợi thành chuỗi JSON với định dạng mong muốn
// Lấy kích thước của hàng đợi
// Kiểm tra hàng đợi có rỗng hay không
// Lấy một phần tử ra khỏi hàng đợi
// Thêm hoặc cập nhật một phần tử vào hàng đợi
{
    class Queue {
        constructor() {
            this.items = [];
        }
    
        // Thêm hoặc cập nhật một phần tử vào hàng đợi
        enqueue(key, value) {
            // Kiểm tra xem key đã tồn tại hay chưa
            for (let i = 0; i < this.items.length; i++) {
                if (this.items[i].key === key) {
                    // Cập nhật giá trị nếu key đã tồn tại
                    this.items[i].value = value;
                    return;
                }
            }
            // Thêm phần tử mới nếu key chưa tồn tại
            this.items.push({ key: key, value: value });
        }
    
        // Lấy một phần tử ra khỏi hàng đợi
        dequeue() {
            if (this.isEmpty()) {
                return "Queue is empty";
            }
            return this.items.shift();
        }
    
        // Kiểm tra hàng đợi có rỗng hay không
        isEmpty() {
            return this.items.length === 0;
        }
    
        // Lấy phần tử đầu tiên của hàng đợi mà không xóa nó
        front() {
            if (this.isEmpty()) {
                return "Queue is empty";
            }
            return this.items[0];
        }
    
        // Lấy kích thước của hàng đợi
        size() {
            return this.items.length;
        }
    
        // In ra tất cả các phần tử trong hàng đợi
        printQueue() {
            let str = "";
            for (let i = 0; i < this.items.length; i++) {
                str += `{ key: ${this.items[i].key}, value: ${this.items[i].value} } `;
            }
            return str;
        }
    
        // Chuyển đổi các phần tử của hàng đợi thành chuỗi JSON với định dạng mong muốn
        toJSON() {
            let jsonObject = {};
            for (let i = 0; i < this.items.length; i++) {
                jsonObject[this.items[i].key] = this.items[i].value;
            }
            return JSON.stringify(jsonObject);
        }
    
        // Chuyển đổi từ chuỗi JSON vào hàng đợi
        fromJSON(jsonString) {
            try {
                const jsonObject = JSON.parse(jsonString);
                for (let key in jsonObject) {
                    if (jsonObject.hasOwnProperty(key)) {
                        this.enqueue(key, jsonObject[key]);
                    }
                }
            } catch (error) {
                console.error("Invalid JSON string:", error);
            }
        }
    
        // Lấy phần tử đầu tiên của hàng đợi mà không xóa nó (peek)
        peek() {
            if (this.isEmpty()) {
                return "Queue is empty";
            }
            return this.items[0];
        }
    }
    
    // Ví dụ sử dụng
    let queue = new Queue();
    queue.enqueue("v1", 10);
    queue.enqueue("v2", 20);
    queue.enqueue("v3", 30);
    console.log(queue.printQueue()); // In ra hàng đợi
    queue.enqueue("v2", 25); // Cập nhật giá trị của key "v2"
    
    console.log(queue.printQueue()); // In ra hàng đợi
    console.log("queue.toJSON()",queue.toJSON()); // Chuyển đổi hàng đợi thành chuỗi JSON với định dạng mong muốn
    
    let jsonString = '{"v4": 40, "v5": 50}';
    queue.fromJSON(jsonString);
    
    console.log(queue.printQueue()); // In ra hàng đợi sau khi thêm từ JSON
    console.log("queue.peek()", queue.peek().key, queue.peek().value); // Lấy phần tử đầu tiên mà không xóa nó
    
}

// Phương thức peekByKey được thêm vào
{
    class Queue {
        constructor() {
            this.items = [];
        }
    
        // Thêm hoặc cập nhật một phần tử vào hàng đợi
        enqueue(key, value) {
            // Kiểm tra xem key đã tồn tại hay chưa
            for (let i = 0; i < this.items.length; i++) {
                if (this.items[i].key === key) {
                    // Cập nhật giá trị nếu key đã tồn tại
                    this.items[i].value = value;
                    return;
                }
            }
            // Thêm phần tử mới nếu key chưa tồn tại
            this.items.push({ key: key, value: value });
        }
    
        // Lấy một phần tử ra khỏi hàng đợi
        dequeue() {
            if (this.isEmpty()) {
                return "Queue is empty";
            }
            return this.items.shift();
        }
    
        // Kiểm tra hàng đợi có rỗng hay không
        isEmpty() {
            return this.items.length === 0;
        }
    
        // Lấy phần tử đầu tiên của hàng đợi mà không xóa nó
        front() {
            if (this.isEmpty()) {
                return "Queue is empty";
            }
            return this.items[0];
        }
    
        // Lấy kích thước của hàng đợi
        size() {
            return this.items.length;
        }
    
        // In ra tất cả các phần tử trong hàng đợi
        printQueue() {
            let str = "";
            for (let i = 0; i < this.items.length; i++) {
                str += `{ key: ${this.items[i].key}, value: ${this.items[i].value} } `;
            }
            return str;
        }
    
        // Chuyển đổi các phần tử của hàng đợi thành chuỗi JSON với định dạng mong muốn
        toJSON() {
            let jsonObject = {};
            for (let i = 0; i < this.items.length; i++) {
                jsonObject[this.items[i].key] = this.items[i].value;
            }
            return JSON.stringify(jsonObject);
        }
    
        // Chuyển đổi từ chuỗi JSON vào hàng đợi
        fromJSON(jsonString) {
            try {
                const jsonObject = JSON.parse(jsonString);
                for (let key in jsonObject) {
                    if (jsonObject.hasOwnProperty(key)) {
                        this.enqueue(key, jsonObject[key]);
                    }
                }
            } catch (error) {
                console.error("Invalid JSON string:", error);
            }
        }
    
        // Lấy phần tử đầu tiên của hàng đợi mà không xóa nó (peek)
        peek() {
            if (this.isEmpty()) {
                return "Queue is empty";
            }
            return this.items[0];
        }
    
        // Lấy giá trị của key mà không xóa phần tử
        peekByKey(key) {
            for (let i = 0; i < this.items.length; i++) {
                if (this.items[i].key === key) {
                    return this.items[i].value;
                }
            }
            return null; // Trả về null nếu không tìm thấy key
        }
    }
    
    // Ví dụ sử dụng
    let queue = new Queue();
    queue.enqueue("v1", 10);
    queue.enqueue("v2", 20);
    queue.enqueue("v3", 30);
    queue.enqueue("v2", 25); // Cập nhật giá trị của key "v2"
    
    console.log(queue.printQueue()); // In ra hàng đợi
    console.log(queue.toJSON()); // Chuyển đổi hàng đợi thành chuỗi JSON với định dạng mong muốn
    
    let jsonString = '{"v4": 40, "v5": 50}';
    queue.fromJSON(jsonString);
    
    console.log(queue.printQueue()); // In ra hàng đợi sau khi thêm từ JSON
    console.log(queue.peekByKey("v2")); // Trả về giá trị của key "v2" mà không xóa phần tử
    console.log(queue.peekByKey("v6")); // Trả về null nếu không tìm thấy key
    
}