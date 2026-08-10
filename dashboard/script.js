const memoryContainer = document.getElementById("memoryContainer");

const heapDisplay = document.getElementById("heapDisplay");
const usedDisplay = document.getElementById("usedDisplay");
const freeDisplay = document.getElementById("freeDisplay");
const fragmentationDisplay =
    document.getElementById("fragmentationDisplay");

const initializeBtn =
    document.getElementById("initializeBtn");


// Temporary memory state
let memory = [];


initializeBtn.addEventListener("click", initializeHeap);


function initializeHeap() {

    const heapSize =
        Number(document.getElementById("heapSize").value);

    const blockSize =
        Number(document.getElementById("blockSize").value);

    if (heapSize <= 0 || blockSize <= 0) {
        alert("Enter valid heap and block sizes.");
        return;
    }

    if (heapSize % blockSize !== 0) {
        alert("Heap size must be divisible by block size.");
        return;
    }

    const blockCount = heapSize / blockSize;

    memory = [];

    for (let i = 0; i < blockCount; i++) {

        memory.push({
            index: i,
            start: i * blockSize,
            size: blockSize,
            allocatedSize: 0,
            free: true
        });
    }

    renderMemory();
}


function renderMemory() {

    memoryContainer.innerHTML = "";

    let used = 0;
    let free = 0;
    let internalFragmentation = 0;

    memory.forEach(block => {

        const element =
            document.createElement("div");

        element.classList.add("memory-block");

        if (block.free) {

            element.classList.add("free");

            free += block.size;

        } else {

            element.classList.add("used");

            used += block.size;

            internalFragmentation +=
                block.size - block.allocatedSize;
        }

        element.innerHTML = `
            <div class="block-index">
                Block ${block.index}
            </div>

            <div class="block-size">
                ${block.allocatedSize || block.size} B
            </div>
        `;

        memoryContainer.appendChild(element);
    });


    heapDisplay.textContent =
        `${used + free} B`;

    usedDisplay.textContent =
        `${used} B`;

    freeDisplay.textContent =
        `${free} B`;

    fragmentationDisplay.textContent =
        `${internalFragmentation} B`;
}