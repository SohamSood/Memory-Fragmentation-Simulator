"use strict";

/* ============================================================
   C++ BACKEND
   ============================================================ */

const API_BASE = "http://localhost:8080";


/* ============================================================
   NAVBAR
   ============================================================ */

const nav = document.getElementById("nav");

if (nav) {
    function handleScroll() {
        if (window.scrollY > 24) {
            nav.classList.add("nav--scrolled");
        } else {
            nav.classList.remove("nav--scrolled");
        }
    }

    handleScroll();

    window.addEventListener("scroll", handleScroll, {
        passive: true
    });
}


/* ============================================================
   MOBILE MENU
   ============================================================ */

const toggle = document.getElementById("navToggle");
const links = document.getElementById("navLinks");

if (toggle && links) {

    toggle.addEventListener("click", function () {

        const isOpen = links.classList.toggle("is-open");

        toggle.classList.toggle("is-open", isOpen);

        toggle.setAttribute(
            "aria-expanded",
            String(isOpen)
        );
    });

    links.querySelectorAll("a").forEach(function (link) {

        link.addEventListener("click", function () {

            links.classList.remove("is-open");
            toggle.classList.remove("is-open");

            toggle.setAttribute(
                "aria-expanded",
                "false"
            );
        });
    });
}


/* ============================================================
   REVEAL ON SCROLL
   ============================================================ */

const reveals = document.querySelectorAll(".reveal");

if ("IntersectionObserver" in window) {

    const revealObserver = new IntersectionObserver(
        function (entries, observer) {

            entries.forEach(function (entry) {

                if (entry.isIntersecting) {

                    entry.target.classList.add("is-visible");

                    observer.unobserve(entry.target);
                }
            });
        },
        {
            threshold: 0.12,
            rootMargin: "0px 0px -40px 0px"
        }
    );

    reveals.forEach(function (element) {
        revealObserver.observe(element);
    });

} else {

    reveals.forEach(function (element) {
        element.classList.add("is-visible");
    });
}


/* ============================================================
   COUNT-UP STATS
   ============================================================ */

const stats = document.querySelectorAll(".stat__num");

function animateCount(element) {

    const target = parseInt(
        element.dataset.target,
        10
    );

    const suffix =
        element.dataset.suffix || "";

    const duration = 1200;

    const start = performance.now();

    function update(now) {

        const progress = Math.min(
            (now - start) / duration,
            1
        );

        const eased =
            1 - Math.pow(1 - progress, 3);

        element.textContent =
            Math.round(target * eased) + suffix;

        if (progress < 1) {
            requestAnimationFrame(update);
        }
    }

    requestAnimationFrame(update);
}


if ("IntersectionObserver" in window) {

    const statObserver = new IntersectionObserver(
        function (entries, observer) {

            entries.forEach(function (entry) {

                if (entry.isIntersecting) {

                    animateCount(entry.target);

                    observer.unobserve(entry.target);
                }
            });
        },
        {
            threshold: 0.5
        }
    );

    stats.forEach(function (element) {
        statObserver.observe(element);
    });

} else {

    stats.forEach(function (element) {

        element.textContent =
            element.dataset.target +
            (element.dataset.suffix || "");
    });
}


/* ============================================================
   GET CURRENT HEAP
   ============================================================ */

async function loadHeap() {

    try {

        const response = await fetch(
            API_BASE + "/"
        );

        if (!response.ok) {
            throw new Error(
                "HTTP error: " + response.status
            );
        }

        const heap = await response.json();

        console.log("Heap received:", heap);

        /*
         * If you already have a renderHeap()
         * function in another JS file, this will use it.
         */

        if (typeof window.renderHeap === "function") {
            window.renderHeap(heap);
        }

        return heap;

    } catch (error) {

        console.error(
            "Could not load heap:",
            error
        );

        return null;
    }
}


/* ============================================================
   ALLOCATE MEMORY
   ============================================================ */

async function allocateMemory(size) {

    size = Number(size);

    if (!Number.isFinite(size) || size <= 0) {

        console.error(
            "Invalid allocation size"
        );

        return;
    }

    try {

        const response = await fetch(
            API_BASE +
            "/allocate?size=" +
            encodeURIComponent(size),
            {
                method: "POST"
            }
        );

        const result =
            await response.text();

        if (!response.ok) {

            throw new Error(result);
        }

        console.log(
            "Allocation successful:",
            result
        );

        /*
         * Get updated heap
         */

        await loadHeap();

    } catch (error) {

        console.error(
            "Allocation failed:",
            error
        );
    }
}


/* ============================================================
   DEALLOCATE MEMORY
   ============================================================ */

async function deallocateMemory(id) {

    id = Number(id);

    if (!Number.isInteger(id) || id < 0) {

        console.error(
            "Invalid allocation ID"
        );

        return;
    }

    try {

        const response = await fetch(
            API_BASE +
            "/deallocate?id=" +
            encodeURIComponent(id),
            {
                method: "DELETE"
            }
        );

        const result =
            await response.text();

        if (!response.ok) {

            throw new Error(result);
        }

        console.log(
            "Deallocation successful:",
            result
        );

        /*
         * Get updated heap
         */

        await loadHeap();

    } catch (error) {

        console.error(
            "Deallocation failed:",
            error
        );
    }
}


/* ============================================================
   MAKE FUNCTIONS AVAILABLE TO HTML
   ============================================================ */

window.allocateMemory = allocateMemory;
window.deallocateMemory = deallocateMemory;
window.loadHeap = loadHeap;


/* ============================================================
   FOOTER YEAR
   ============================================================ */

const year = document.getElementById("year");

if (year) {
    year.textContent =
        String(new Date().getFullYear());
}


/* ============================================================
   START
   ============================================================ */

loadHeap();