--
-- PostgreSQL database dump
--

-- Dumped from database version 14.10 (Ubuntu 14.10-0ubuntu0.22.04.1)
-- Dumped by pg_dump version 14.10 (Ubuntu 14.10-0ubuntu0.22.04.1)

-- Started on 2024-01-14 16:44:12 CET

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 4283 (class 1262 OID 16384)
-- Name: airspace; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE airspace WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE = 'en_US.UTF-8';


ALTER DATABASE airspace OWNER TO postgres;

\connect airspace

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 5 (class 2615 OID 2200)
-- Name: public; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA public;


ALTER SCHEMA public OWNER TO postgres;

--
-- TOC entry 4284 (class 0 OID 0)
-- Dependencies: 5
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'standard public schema';


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 220 (class 1259 OID 17458)
-- Name: airspace; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.airspace (
    id integer NOT NULL,
    insert_timestamp timestamp with time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    designator text,
    airspace_element_type text,
    centroid_lon json,
    centroid_lat json,
    st_distance double precision,
    epoch_from numeric,
    epoch_to numeric,
    lower_altitude text,
    upper_altitude text,
    unit text,
    remarks text,
    geography public.geography,
    json_body json
);


ALTER TABLE public.airspace OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 17457)
-- Name: airspace_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.airspace_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.airspace_id_seq OWNER TO postgres;

--
-- TOC entry 4285 (class 0 OID 0)
-- Dependencies: 219
-- Name: airspace_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.airspace_id_seq OWNED BY public.airspace.id;


--
-- TOC entry 4126 (class 2604 OID 17461)
-- Name: airspace id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.airspace ALTER COLUMN id SET DEFAULT nextval('public.airspace_id_seq'::regclass);


--
-- TOC entry 4277 (class 0 OID 17458)
-- Dependencies: 220
-- Data for Name: airspace; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.airspace (id, insert_timestamp, designator, airspace_element_type, centroid_lon, centroid_lat, st_distance, epoch_from, epoch_to, lower_altitude, upper_altitude, unit, remarks, geography, json_body) FROM stdin;
\.


--
-- TOC entry 4124 (class 0 OID 16695)
-- Dependencies: 212
-- Data for Name: spatial_ref_sys; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.spatial_ref_sys (srid, auth_name, auth_srid, srtext, proj4text) FROM stdin;
\.


--
-- TOC entry 4286 (class 0 OID 0)
-- Dependencies: 219
-- Name: airspace_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.airspace_id_seq', 1, true);


--
-- TOC entry 4131 (class 2606 OID 17466)
-- Name: airspace airspace_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.airspace
    ADD CONSTRAINT airspace_pkey PRIMARY KEY (id);


-- Completed on 2024-01-14 16:44:13 CET

--
-- PostgreSQL database dump complete
--

